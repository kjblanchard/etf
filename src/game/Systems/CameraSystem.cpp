#include <Components/PlayerComponent.hpp>
#include <Supergoon/pch.hpp>
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
// static Tween* tweener = nullptr;
static Sequence* sequence = nullptr;

void InitializeCamera() {
	// TODO , this needs memory management.
	sequence = new Sequence();
	auto tweener = std::make_shared<Tween>(255, 0, 1.0, &colorFade, Supergoon::Easings::Linear);
	tweener->UpdateFunc = []() {
		auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
		auto g = GameObject::GetGameObjectWithComponents<GameState>();
		auto& cc = c->GetComponent<CameraComponent>();
		auto& gc = g->GetComponent<GameState>();
		if (gc.EnteringBattle) {
			cc.Box.X -= 5;
			auto color = Color{255, (uint8_t)colorFade, (uint8_t)colorFade, (uint8_t)colorFade};
			Level::SetBackGroundColor(color);
			Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void*)&cc.Box);
			return;
		};
	};
	sequence->Tweens.push_back(tweener);
	auto waitTween = std::make_shared<Tween>(0.1);
	sequence->Tweens.push_back(waitTween);
	waitTween->EndFunc = []() {
		// auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
		auto g = GameObject::GetGameObjectWithComponents<GameState>();
		auto& gc = g->GetComponent<GameState>();
		gc.EnteringBattle = false;
		gc.InBattle = true;
		gc.Loading = true;
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void*)strdup(("forest1")));
		sequence->Restart();
	};
}
void UpdateCamera() {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto g = GameObject::GetGameObjectWithComponents<GameState>();
	if (!c.has_value() || !g.has_value()) {
		return;
	}
	auto& cc = c->GetComponent<CameraComponent>();
	auto& gc = g->GetComponent<GameState>();
	if (gc.EnteringBattle) {
		sequence->Update();
		return;
	}
	if (!gc.CameraFollowTarget) {
		return;
	}
	if (!cc.FollowTarget) {
		getFollowTarget(cc);
	}
	auto pl = cc.FollowTarget;
	if (!pl) {
		sgLogError("Trying to follow a target, but no target is found!");
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
	Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void*)&cc.Box);
}
}  // namespace Supergoon
