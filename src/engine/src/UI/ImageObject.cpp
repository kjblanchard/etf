#include <Supergoon/UI/ImageObject.hpp>
// #include <Supergoon/Graphics/Graphics.hpp>

using namespace Supergoon;
void ImageObject::Draw() {
	Image->Draw(ImageSourceRect, Bounds);
}
