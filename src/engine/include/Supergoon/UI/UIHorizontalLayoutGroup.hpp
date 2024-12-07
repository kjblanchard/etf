#pragma once
#include <Supergoon/UI/UIObject.hpp>
namespace Supergoon {
class Panel;
class UIHorizontalLayoutGroup : public UIObject {
   public:
	UIHorizontalLayoutGroup(UIObject* parent, std::string id);
	virtual void Draw() override;
	virtual void OnDirty() override;
	float YSpaceBetweenElements = 0;

   private:
	std::vector<UIObject*> _drawOrder;
	friend UIWidget;
};
}  // namespace Supergoon
