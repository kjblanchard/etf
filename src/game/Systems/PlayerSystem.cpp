#include <Components/AnimationComponent.hpp>
#include <Components/Image.hpp>
#include <Components/Location.hpp>
#include <Components/Player.hpp>
#include <Components/PlayerSpawnComponent.hpp>
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/CameraComponent.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/ECS/GameStateComponent.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Systems/PlayerSystem.hpp>
using namespace Supergoon;

static void loadPlayer(GameObject, PlayerSpawnComponent& playerSpawn, GameState& gameState) {
	auto go = new GameObject();
	auto playerRb = RigidbodyComponent(*gameState.CurrentLevel);
	// auto playerImage = ImageComponent();
	auto playerLocation = LocationComponent();
	auto playerComponent = PlayerComponent();
	auto playerAnimation = AnimationComponent();
	playerAnimation.AnimationName = "player" + std::to_string(playerComponent.PlayerNum + 1);
	playerAnimation.Offset = gePoint{0, 0};
	playerAnimation.AnimationSpeed = 1.0;
	// playerImage.Image = ContentRegistry::CreateContent<Image>("assets/img/player1.png");
	playerComponent.PlayerNum = 0;
	playerComponent.Direction = Directions::South;

	go->AddComponent<AnimationComponent>(playerAnimation);
	go->AddComponent<LocationComponent>(playerLocation);
	go->AddComponent<PlayerComponent>(playerComponent);
	go->AddComponent<RigidbodyComponent>(playerRb);
	// go->AddComponent<ImageComponent>(playerImage);
	gameState.CurrentLevel->AddGameObjectToLevel(go);
}
static void playerInput(GameObject go, PlayerComponent& player) {
	auto vel = Vector2();
	auto& rb = go.GetComponent<RigidbodyComponent>();
	auto& anim = go.GetComponent<AnimationComponent>();
	auto speed = 100;
	auto moved = false;
	auto newDirection = player.Direction;
	if (KeyDown(KeyboardKeys::Key_S)) {
		vel.Y -= speed;
		moved = true;
		newDirection = Directions::South;
	}
	if (KeyDown(KeyboardKeys::Key_D)) {
		vel.X += speed;
		moved = true;
		newDirection = Directions::East;
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		vel.Y += speed;
		moved = true;
		newDirection = Directions::North;
	}
	if (KeyDown(KeyboardKeys::Key_A)) {
		vel.X -= speed;
		moved = true;
		newDirection = Directions::West;
	}
	rb.SetVelocity(vel);
	anim.Playing = moved;
	if (newDirection != player.Direction) {
		// Update the Animation for the new direction
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
