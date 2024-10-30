#pragma once
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <SupergoonEngine/nlohmann/json_fwd.hpp>
namespace Supergoon {
class Panel;
class ImageObject : public UIObject {
   public:
	ImageObject(Panel* parent, nlohmann::json& json);
	RectangleF ImageSourceRect;
	int Transparency = 255;
	std::shared_ptr<Image> ImagePtr;
	virtual void Draw() override;
	virtual void OnDirty() override;
};
}  // namespace Supergoon
