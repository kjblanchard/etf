// #include <BbAdventures/base/GameObject.hpp>
// #include <BbAdventures/components/CameraComponent.hpp>
// #include <BbAdventures/components/LocationComponent.hpp>
// #include <BbAdventures/components/PlayerComponent.hpp>
// #include <BbAdventures/shared/constants.hpp>
// #include <BbAdventures/shared/state.hpp>
#include <Supergoon/ECS/Location.hpp>
#include <Components/Player.hpp>
#include <Supergoon/ECS/CameraComponent.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/ECS/GameStateComponent.hpp>
#include <algorithm>
namespace Supergoon {
int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 288;

void getFollowTarget(CameraComponent& cc) {
	GameObject::ForEach<PlayerComponent, LocationComponent>([&cc](GameObject, PlayerComponent& pc, LocationComponent& lc) {
		if (pc.PlayerNum != 0) {
			return;
		}
		cc.FollowTarget = &lc;
	});
}

void UpdateCamera() {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto g = GameObject::GetGameObjectWithComponents<GameState>();
	if (!c.has_value() || !g.has_value()) {
		return;
	}
	auto& cc = c->GetComponent<CameraComponent>();
	auto& gc = c->GetComponent<GameState>();
	if (!cc.FollowTarget) {
		getFollowTarget(cc);
	}
	auto pl = cc.FollowTarget;
	cc.Box.X = pl->Location.X - (SCREEN_WIDTH / 2);
	cc.Box.Y = pl->Location.Y - (SCREEN_HEIGHT / 2);
	if (cc.Box.X < 0) {
		cc.Box.X = 0;
	} else if (cc.Box.X > cc.Bounds.x - SCREEN_WIDTH) {
		cc.Box.X = std::max(cc.Bounds.x - SCREEN_WIDTH, 0);
	}
	if (cc.Box.Y < 0) {
		cc.Box.Y = 0;
	} else if (cc.Box.Y > (cc.Bounds.y - SCREEN_HEIGHT)) {
		cc.Box.Y = cc.Bounds.y - SCREEN_HEIGHT;
	}

	gc.CurrentLevel->cameraX = (int)cc.Box.X;
	gc.CurrentLevel->cameraY = (int)cc.Box.Y;
}
}  // namespace Supergoon
