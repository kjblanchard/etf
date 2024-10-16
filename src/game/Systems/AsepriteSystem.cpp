// #include <BbAdventures/aseprite/AsepriteAnimation.hpp>
// #include <BbAdventures/base/GameObject.hpp>
// #include <BbAdventures/components/AnimationComponent.hpp>
// #include <BbAdventures/components/LocationComponent.hpp>
// #include <BbAdventures/shared/state.hpp>
#include <Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Location.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/CameraComponent.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/ECS/GameStateComponent.hpp>
#include <Supergoon/Game.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <memory>
using namespace Supergoon;
void loadAnimationComponent(AnimationComponent& a) {
	a.Animation = new AsepriteAnimation(a.AnimationName);
	a.AnimationImage = ContentRegistry::CreateContent<Image>(a.Animation->Filename());
	// a.AnimationImage = geImageNewFromFile(a.Animation->Filename().c_str());
}

void Supergoon::LoadAnimationComponents() {
	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
		loadAnimationComponent(a);
	});
}
void Supergoon::UpdateAnimationComponents() {
	auto deltatime = (float)Game::Instance()->DeltaTime();
	auto msTime = deltatime * 1000;

	GameObject::ForEach<AnimationComponent>([&msTime](GameObject, AnimationComponent& a) {
		if (a.Animation == nullptr) {
			loadAnimationComponent(a);
		}
		if (!a.Playing) {
			return;
		}
		a.Animation->UpdateAnimation(msTime * a.AnimationSpeed);
	});
}
void Supergoon::DrawAnimationComponents() {
	GameObject::ForEach<AnimationComponent, LocationComponent>([](GameObject, AnimationComponent& a, LocationComponent& l) {
		auto s = a.Animation->FrameCoords();
		auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
		auto& cc = c->GetComponent<CameraComponent>();
		// TODO why do we need to get camera here.
		// 0 is for camera X and Camera Y
		auto d = RectangleF{(float)(int)(l.Location.X + a.Offset.x) - (int)cc.Box.X, (float)(int)(l.Location.Y + a.Offset.y) - (int)cc.Box.Y, (float)s.W, (float)s.H};
		a.AnimationImage->Draw(s, d);
	});
}
void Supergoon::FreeAnimationComponents() {
}

// void DrawAnimationComponents() {
// }

// void FreeAnimationComponents() {
// 	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
// 		delete (a.Animation);
// 	});
// }
// }  // namespace Bba
