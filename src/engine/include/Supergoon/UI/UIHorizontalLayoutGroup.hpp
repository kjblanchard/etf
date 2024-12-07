#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/UI/UIObject.hpp>
namespace Supergoon {
class Panel;
class UIImage : public UIObject {
   public:
	//  If you want to initialize everything about this Object.
	HorizontalLayoutGroup(Panel* parent, std::string id);
	RectangleF ImageSourceRect = {0, 0, 0, 0};
	virtual void Draw() override;
	virtual void OnDirty() override;
};
}  // namespace Supergoon
