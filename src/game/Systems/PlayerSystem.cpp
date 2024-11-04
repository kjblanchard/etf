#include <Components/PlayerComponent.hpp>
#include <Components/PlayerExitComponent.hpp>
#include <Components/PlayerSpawnComponent.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/PlayerSystem.hpp>
using namespace Supergoon;

static void loadPlayer(GameObject, PlayerSpawnComponent& playerSpawn, GameState& gameState) {
	auto go = new GameObject();
	auto playerLocation = LocationComponent();
	auto playerComponent = PlayerComponent();
	auto playerAnimation = AnimationComponent();
	playerAnimation.AnimationName = "player" + std::to_string(playerComponent.PlayerNum + 1);
	playerAnimation.Offset = Point{0, 0};
	playerAnimation.AnimationSpeed = 1.0;
	playerComponent.PlayerNum = 0;
	playerComponent.Direction = Directions::South;
	playerComponent.Body = RectangleF{4, 7, 16, 27};
	playerLocation.Location.X = playerSpawn.Location.X;
	playerLocation.Location.Y = playerSpawn.Location.Y;
	go->AddComponent<AnimationComponent>(playerAnimation);
	go->AddComponent<LocationComponent>(playerLocation);
	go->AddComponent<PlayerComponent>(playerComponent);
	gameState.CurrentLevel->AddGameObjectToLevel(go);
}

static void playerInput(GameObject go, PlayerComponent& player) {
	auto state = GameObject::GetGameObjectWithComponents<GameState>();
	auto& stateComponent = state->GetComponent<GameState>();
	if (!state.has_value()) {
		sgLogError("No gamestate component somehow");
		return;
	}
	if (stateComponent.Loading) {
		return;
	}
	auto vel = Vector2();
	auto& anim = go.GetComponent<AnimationComponent>();
	auto& loc = go.GetComponent<LocationComponent>();
	auto speed = 100;
	auto moved = false;
	auto newDirection = player.Direction;
	//
	// Handle button presses
	if (KeyDown(KeyboardKeys::Key_S)) {
		vel.Y += speed;
		moved = true;
		newDirection = Directions::South;
	}
	if (KeyDown(KeyboardKeys::Key_D)) {
		vel.X += speed;
		moved = true;
		newDirection = Directions::East;
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		vel.Y -= speed;
		moved = true;
		newDirection = Directions::North;
	}
	if (KeyDown(KeyboardKeys::Key_A)) {
		vel.X -= speed;
		moved = true;
		newDirection = Directions::West;
	}
	auto deltatime = (float)Game::DeltaTime();
	vel *= Vector2{deltatime, deltatime};
	// Handle Collisions
	auto desiredPosition = loc.Location;
	desiredPosition.X += vel.X;
	desiredPosition.Y += vel.Y;
	GameObject::ForEach<SolidComponent, LocationComponent>([&desiredPosition, &player](GameObject, SolidComponent s, LocationComponent l) {
		auto playerBodyRect = RectangleF{desiredPosition.X + player.Body.X, desiredPosition.Y + player.Body.Y, player.Body.W, player.Body.H};
		auto bcf = RectangleF{l.Location.X, l.Location.Y, (float)s.Size.X, (float)s.Size.Y};
		if (playerBodyRect.IsOverlap(&bcf)) {
			auto desiredPlayerOverlapRect = playerBodyRect.GetOverlapRect(&bcf);
			auto direction = GetOverlapDirectionF(&playerBodyRect, &desiredPlayerOverlapRect);
			switch (direction) {
				case Directions::North:
					desiredPosition.Y += desiredPlayerOverlapRect.H;
					break;
				case Directions::East:
					desiredPosition.X -= desiredPlayerOverlapRect.W;
					break;
				case Directions::South:
					desiredPosition.Y -= desiredPlayerOverlapRect.H;
					break;
				case Directions::West:
					desiredPosition.X += desiredPlayerOverlapRect.W;
					break;
				default:
					break;
			}
		}
	});
	// loc.Location += vel;
	if (moved) {
		loc.Location.X = std::round(desiredPosition.X);
		loc.Location.Y = std::round(desiredPosition.Y);
	}
	anim.Playing = moved;
	if (newDirection != player.Direction) {
		auto letter = GetLetterForDirection(newDirection);
		anim.Animation->PlayAnimation("walk" + std::string(letter));
		player.Direction = newDirection;
	}

	// Did we exit?
	auto playerBodyRect = RectangleF{loc.Location.X + player.Body.X, loc.Location.Y + player.Body.Y, player.Body.W, player.Body.H};
	auto exited = false;
	GameObject::ForEach<PlayerExitComponent>([&](GameObject, PlayerExitComponent pe) {
		if (exited) {
			return;
		}
		if (playerBodyRect.IsOverlap(&pe.BoundingBox)) {
			// Play transition sound
			stateComponent.PlayerSpawnLocation = pe.SpawnLocationId;
			Level::LoadNewLevelFade(pe.NextMap);
			Level::LoadLocation = pe.SpawnLocationId;
			stateComponent.Loading = true;
			exited = true;
		}
	});
	if (exited) {
		return;
	}
}

static void loadPlayerEach(GameObject go, PlayerSpawnComponent& ps) {
	auto state = GameObject::GetGameObjectWithComponents<GameState>();
	if (!state.has_value()) {
		return;
	}
	auto& stateComponent = state->GetComponent<GameState>();

	if (ps.SpawnLocationId != stateComponent.PlayerSpawnLocation) {
		return;
	}

	loadPlayer(go, ps, stateComponent);
}

void Supergoon::LoadPlayers() {
	GameObject::ForEach<PlayerSpawnComponent>(loadPlayerEach);
}
void Supergoon::PlayerInput() {
	GameObject::ForEach<PlayerComponent>(playerInput);
}
