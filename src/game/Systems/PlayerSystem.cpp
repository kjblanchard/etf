#include <Components/PlayerComponent.hpp>
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
	playerComponent.Body = RectangleF{2, 2, 20, 20};
	playerLocation.Location.X = playerSpawn.Location.X;
	playerLocation.Location.Y = playerSpawn.Location.Y;
	go->AddComponent<AnimationComponent>(playerAnimation);
	go->AddComponent<LocationComponent>(playerLocation);
	go->AddComponent<PlayerComponent>(playerComponent);
	gameState.CurrentLevel->AddGameObjectToLevel(go);
}
static void playerInput(GameObject go, PlayerComponent& player) {
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
	auto deltatime = (float)Game::Instance()->DeltaTime();
	vel *= Vector2{deltatime, deltatime};
	//
	// Handle Collisions
	auto desiredPosition = loc.Location;
	desiredPosition.X += vel.X;
	desiredPosition.Y += vel.Y;
	GameObject::ForEach<SolidComponent, LocationComponent>([&desiredPosition, &player](GameObject, SolidComponent s, LocationComponent l) {
		auto playerBodyRect = RectangleF{desiredPosition.X + player.Body.X, desiredPosition.Y + player.Body.Y, player.Body.W, player.Body.H};
		auto bcf = RectangleF{l.Location.X, l.Location.Y, (float)s.Size.X, (float)s.Size.Y};
		if (playerBodyRect.IsOverlap(&bcf)) {
			auto r = playerBodyRect.GetOverlapRect(&bcf);
			auto d = GetOverlapDirectionF(&playerBodyRect, &r);
			switch (d) {
				case Directions::North:
					desiredPosition.Y += r.H;
					// collision = true;
					break;
				case Directions::East:
					desiredPosition.X -= r.W;
					// collision = true;
					break;
				case Directions::South:
					desiredPosition.Y -= r.H;
					// collision = true;
					break;
				case Directions::West:
					desiredPosition.X += r.W;
					// collision = true;
					break;
				default:
					break;
			}
		}
	});
	// loc.Location += vel;
	if (moved) {
		loc.Location.X = desiredPosition.X;
		loc.Location.Y = desiredPosition.Y;
	}
	anim.Playing = moved;
	if (newDirection != player.Direction) {
		auto letter = GetLetterForDirection(newDirection);
		anim.Animation->PlayAnimation("walk" + std::string(letter));
		player.Direction = newDirection;
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
