#pragma once
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/UI/UIObject.hpp>
namespace Supergoon {
class ImageObject : public UIObject {
   public:
	RectangleF ImageSourceRect;
	std::shared_ptr<Image> Image;
	virtual void Draw() override;
};
}  // namespace Supergoon
