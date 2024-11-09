#include <Components/PlayerComponent.hpp>
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
float colorFade = 255;
static Tween* tweener = new Tween(255, 0, 1.0, &colorFade, Supergoon::Easings::Linear);

void UpdateCamera() {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto g = GameObject::GetGameObjectWithComponents<GameState>();
	if (!c.has_value() || !g.has_value()) {
		return;
	}
	auto& cc = c->GetComponent<CameraComponent>();
	auto& gc = g->GetComponent<GameState>();
	if (gc.EnteringBattle) {
		// We should instead slowly decrement camera x
		tweener->Update();
		cc.Box.X -= 5;

		auto color = Color{255, (uint8_t)colorFade, (uint8_t)colorFade, (uint8_t)colorFade};
		Level::SetBackGroundColor(color);
		Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void*)&cc.Box);
		return;
	}
	if (!gc.CameraFollowTarget) {
		return;
	}
	if (!cc.FollowTarget) {
		getFollowTarget(cc);
		tweener->Restart();
	}
	auto pl = cc.FollowTarget;
	if (!pl) {
		sgLogError(" no follow boi");
		Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void*)&cc.Box);
		return;
	}
	cc.Box.X = (int)(pl->Location.X - (gc.WindowWidth / 2.0f));
	cc.Box.Y = (int)(pl->Location.Y - (gc.WindowHeight / 2.0f));
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

	// gc.CurrentLevel->cameraX = (int)cc.Box.X;
	// gc.CurrentLevel->cameraY = (int)cc.Box.Y;
	Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void*)&cc.Box);
	// gc.CurrentLevel->cameraX = cc.Box.X;
	// gc.CurrentLevel->cameraY = cc.Box.Y;
}
}  // namespace Supergoon
