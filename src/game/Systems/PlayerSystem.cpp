#include <Components/GameStateComponent.hpp>
#include <Components/Image.hpp>
#include <Components/Player.hpp>
#include <Components/PlayerSpawnComponent.hpp>
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Systems/PlayerSystem.hpp>
using namespace Supergoon;

static void loadPlayer(GameObject, PlayerSpawnComponent& playerSpawn, GameState& gameState) {
	auto go = new GameObject();
	auto playerRb = RigidbodyComponent(*gameState.CurrentLevel);
	auto playerImage = ImageComponent();
	// playerImage.Image = ContentRegistry::CreateContent<Image>("assets/img/player1.png").get();
	playerImage.Image = ContentRegistry::CreateContent<Image>("assets/img/player1.png");
	// playerImage = thing;
	// LocationComponent l = LocationComponent();
	// l.Location.x = ps.Location.x + (_playerLoadNum * 5);
	// l.Location.y = ps.Location.y;
	auto playerComponent = PlayerComponent();
	playerComponent.PlayerNum = 0;
	// p.Direction = ps.SpawnDirection;
	// p.PlayerNum = _playerLoadNum;
	// p.ControllerNum = State::PlayerControllerMap[p.PlayerNum];
	// auto a = AnimationComponent();
	// a.AnimationName = "player" + std::to_string(p.PlayerNum + 1);
	// a.Offset = gePoint{0, 0};
	// auto r = RigidBodyComponent();
	// r.OffsetX = 8;
	// r.OffsetY = 20;
	// r.W = 10;
	// r.H = 14;
	// auto ic = InteractorComponent();
	// ic.Box = _interactionRect;
	// auto dd = DebugDrawComponent();
	// dd.Box = _interactionRect;
	go->AddComponent<PlayerComponent>(playerComponent);
	go->AddComponent<RigidbodyComponent>(playerRb);
	go->AddComponent<ImageComponent>(playerImage);
	// go->AddComponent<LocationComponent>(l);
	// go->AddComponent<PlayerComponent>(p);
	// go->AddComponent<AnimationComponent>(a);
	// go->AddComponent<InteractorComponent>(ic);
	// // go->AddComponent<DebugDrawComponent>(dd);
	// State::CurrentLevel->AddGameObjectToLevel(go);
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
