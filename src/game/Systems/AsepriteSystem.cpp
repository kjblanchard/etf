// #include <BbAdventures/aseprite/AsepriteAnimation.hpp>
// #include <BbAdventures/base/GameObject.hpp>
// #include <BbAdventures/components/AnimationComponent.hpp>
// #include <BbAdventures/components/LocationComponent.hpp>
// #include <BbAdventures/shared/state.hpp>
#include <Components/AnimationComponent.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <memory>
using namespace Supergoon;
void loadAnimationComponent(AnimationComponent& a) {
	a.Animation = new AsepriteAnimation(a.AnimationName);
	// a.AnimationImage = geImageNewFromFile(a.Animation->Filename().c_str());
}

void Supergoon::LoadAnimationComponents() {
	// 	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
	// 		loadAnimationComponent(a);
	// }
}
void Supergoon::UpdateAnimationComponents() {
}
void Supergoon::DrawAnimationComponents() {
}
void Supergoon::FreeAnimationComponents() {
}

// void UpdateAnimationComponents() {
// 	auto msTime = State::DeltaTime * 1000;

// 	GameObject::ForEach<AnimationComponent>([&msTime](GameObject, AnimationComponent& a) {
// 		if(a.Animation == nullptr) {
// 			loadAnimationComponent(a);
// 		}
// 		if (!a.Playing) {
// 			return;
// 		}
// 		a.Animation->UpdateAnimation(msTime * a.AnimationSpeed);
// 	});
// }

// void DrawAnimationComponents() {
// 	GameObject::ForEach<AnimationComponent, LocationComponent>([](GameObject, AnimationComponent& a, LocationComponent& l) {
// 		auto s = a.Animation->FrameCoords();
// 		auto d = geRectangleF{(l.Location.x + a.Offset.x) - State::CameraX, (l.Location.y + a.Offset.y) - State::CameraY, (float)s.w, (float)s.h};
// 		geImageDrawF(a.AnimationImage, &s, &d);
// 	});
// }

// void FreeAnimationComponents() {
// 	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
// 		delete (a.Animation);
// 	});
// }
// }  // namespace Bba
