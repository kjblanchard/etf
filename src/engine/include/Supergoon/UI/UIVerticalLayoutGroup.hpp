#pragma once
#include <Supergoon/UI/UIObject.hpp>
namespace Supergoon {
class Panel;
class UIVerticalLayoutGroup : public UIObject {
   public:
	UIVerticalLayoutGroup(UIObject* parent, std::string id);
	virtual void Draw() override;
	virtual void OnDirty() override;
	float XSpaceBetweenElements = 0;

   private:
	std::vector<UIObject*> _drawOrder;
	friend UIWidget;
};
}  // namespace Supergoon
