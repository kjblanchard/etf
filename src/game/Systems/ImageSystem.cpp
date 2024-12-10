#include <Supergoon/ECS/GameObject.h>

#include <Supergoon/ECS/Components/ImageComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Systems/ImageSystem.hpp>
using namespace Supergoon;
static void drawImageEach(GameObject, ImageComponent& imageComponent, LocationComponent& locationComponent) {
	auto dst = RectangleF{(float)(int)locationComponent.Location.X, (float)(int)locationComponent.Location.Y, 27, 31};
	imageComponent.ImagePtr->Draw(imageComponent.SourceRect, dst);
}

void Supergoon::DrawImages() {
	// GameObject::ForEach<ImageComponent, LocationComponent>(drawImageEach);
}
