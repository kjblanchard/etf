#include <Components/Player.hpp>
#include <Supergoon/Supergoon.hpp>
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
	} else if (cc.Box.X > cc.Bounds.X - SCREEN_WIDTH) {
		cc.Box.X = std::max(cc.Bounds.X - SCREEN_WIDTH, 0);
	}
	if (cc.Box.Y < 0) {
		cc.Box.Y = 0;
	} else if (cc.Box.Y > (cc.Bounds.Y - SCREEN_HEIGHT)) {
		cc.Box.Y = cc.Bounds.Y - SCREEN_HEIGHT;
	}

	gc.CurrentLevel->cameraX = (int)cc.Box.X;
	gc.CurrentLevel->cameraY = (int)cc.Box.Y;
}
}  // namespace Supergoon
