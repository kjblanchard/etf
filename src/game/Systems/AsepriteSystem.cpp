#include <Supergoon/Supergoon.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <memory>
using namespace Supergoon;
void loadAnimationComponent(AnimationComponent& a) {
	a.Animation = new AsepriteAnimation(a.AnimationName);
	a.AnimationImage = ContentRegistry::CreateContent<Image>(a.Animation->Filename());
}

void Supergoon::LoadAnimationComponents() {
	GameObject::ForEach<AnimationComponent>([](GameObject, AnimationComponent& a) {
		loadAnimationComponent(a);
	});
}
void Supergoon::UpdateAnimationComponents() {
	auto msTime = Game::Instance()->DeltaTimeMS();
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
		auto d = RectangleF{(float)(int)(l.Location.X + a.Offset.X) - (int)cameraComponent.Box.X, (float)(int)(l.Location.Y + a.Offset.Y) - (int)cameraComponent.Box.Y, (float)srcRect.W, (float)srcRect.H};
		a.AnimationImage->Draw(srcRect, d);
	});
}
void Supergoon::FreeAnimationComponents() {
}
