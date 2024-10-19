#include "ImageSystem.hpp"

#include <Components/Image.hpp>
#include <Systems/ImageSystem.hpp>
#include <Supergoon/Supergoon.hpp>
using namespace Supergoon;
static void drawImageEach(GameObject go, ImageComponent& imageComponent, LocationComponent& locationComponent) {
	auto src = RectangleF{0, 0, 27, 31};
	auto dst = RectangleF{(float)(int)locationComponent.Location.X, (float)(int)locationComponent.Location.Y, 27, 31};
	imageComponent.Image->Draw(src, dst);
}

void Supergoon::DrawImages() {
	GameObject::ForEach<ImageComponent, LocationComponent>(drawImageEach);
}
