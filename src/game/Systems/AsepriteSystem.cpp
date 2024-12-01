#include <Supergoon/Supergoon.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <memory>
using namespace Supergoon;

void loadAnimationComponent(AnimationComponent& a) {
	if (!a.Animation) {
		a.Animation = std::make_shared<AsepriteAnimation>(a.AnimationName);
	}
	a.AnimationImage = ContentRegistry::CreateContent<Image>(a.Animation->Filename());
}

void Supergoon::LoadAnimationComponents() {
	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
		loadAnimationComponent(a);
	});
}
void Supergoon::UpdateAnimationComponents() {
	auto msTime = Game::DeltaTimeMS();
	GameObject::ForEach<AnimationComponent>([&msTime](GameObject, AnimationComponent& a) {
		if (!a.Playing) {
			return;
		}
		a.Animation->UpdateAnimation(msTime * a.AnimationSpeed);
	});
}
void Supergoon::DrawAnimationComponents() {
	auto cameraGO = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cameraComponent = cameraGO->GetComponent<CameraComponent>();
	GameObject::ForEach<AnimationComponent, LocationComponent>([&cameraComponent](GameObject, AnimationComponent& a, LocationComponent& l) {
		auto srcRect = a.Animation->FrameCoords();
		auto dWidth = a.OverrideDrawSize.X ? a.OverrideDrawSize.X : srcRect.W;
		auto dHeight = a.OverrideDrawSize.Y ? a.OverrideDrawSize.Y : srcRect.H;
		auto d = RectangleF{(float)(int)(l.Location.X + a.Offset.X) - (int)cameraComponent.Box.X,
							(float)(int)(l.Location.Y + a.Offset.Y) - (int)cameraComponent.Box.Y,
							(float)dWidth,
							(float)dHeight};
		a.AnimationImage->Draw(srcRect, d);
	});
}
