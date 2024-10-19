#include <Components/Player.hpp>
#include <Supergoon/Supergoon.hpp>
#include <algorithm>
namespace Supergoon {

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
	cc.Box.X = pl->Location.X - (gc.WindowWidth / 2);
	cc.Box.Y = pl->Location.Y - (gc.WindowHeight / 2);
	if (cc.Box.X < 0) {
		cc.Box.X = 0;
	} else if (cc.Box.X > cc.Bounds.X - gc.WindowWidth) {
		cc.Box.X = std::max(cc.Bounds.X - gc.WindowWidth, 0);
	}
	if (cc.Box.Y < 0) {
		cc.Box.Y = 0;
	} else if (cc.Box.Y > (cc.Bounds.Y - gc.WindowHeight)) {
		cc.Box.Y = cc.Bounds.Y - gc.WindowHeight;
	}

	gc.CurrentLevel->cameraX = (int)cc.Box.X;
	gc.CurrentLevel->cameraY = (int)cc.Box.Y;
}
}  // namespace Supergoon
