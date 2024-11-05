#pragma once
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <SupergoonEngine/nlohmann/json_fwd.hpp>
namespace Supergoon {
class Panel;
class ImageObject : public UIObject {
   public:
	//  If you want to initialize everything about this Object.
	ImageObject(Panel* parent);
	// If you are loading from a ui json file.
	ImageObject(Panel* parent, nlohmann::json& json);
	RectangleF ImageSourceRect;
	int Transparency = 255;
	std::shared_ptr<Image> ImagePtr;
	virtual void Draw() override;
	virtual void OnDirty() override;
};
}  // namespace Supergoon
