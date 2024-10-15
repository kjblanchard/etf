#include <Components/Image.hpp>
#include <Components/Location.hpp>
#include <Components/Player.hpp>
#include <Components/PlayerSpawnComponent.hpp>
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/ECS/GameStateComponent.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Systems/PlayerSystem.hpp>
using namespace Supergoon;

static void loadPlayer(GameObject, PlayerSpawnComponent& playerSpawn, GameState& gameState) {
	auto go = new GameObject();
	auto playerRb = RigidbodyComponent(*gameState.CurrentLevel);
	auto playerImage = ImageComponent();
	auto playerLocation = LocationComponent();
	playerImage.Image = ContentRegistry::CreateContent<Image>("assets/img/player1.png");
	auto playerComponent = PlayerComponent();
	playerComponent.PlayerNum = 0;
	go->AddComponent<LocationComponent>(playerLocation);
	go->AddComponent<PlayerComponent>(playerComponent);
	go->AddComponent<RigidbodyComponent>(playerRb);
	go->AddComponent<ImageComponent>(playerImage);
	gameState.CurrentLevel->AddGameObjectToLevel(go);
}
static void playerInput(GameObject go, PlayerComponent&) {
	auto vel = Vector2();
	auto& rb = go.GetComponent<RigidbodyComponent>();
	auto speed = 100;
	if (KeyDown(KeyboardKeys::Key_S)) {
		vel.Y -= speed;
	}
	if (KeyDown(KeyboardKeys::Key_D)) {
		vel.X += speed;
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		vel.Y += speed;
	}
	if (KeyDown(KeyboardKeys::Key_A)) {
		vel.X -= speed;
	}
	rb.SetVelocity(vel);
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
