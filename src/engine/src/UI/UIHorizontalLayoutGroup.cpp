#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/pch.hpp>

using namespace Supergoon;

UIHorizontalLayoutGroup::UIHorizontalLayoutGroup(UIObject* parent, std::string name) : UIObject(parent) {
	WidgetType = (int)BuiltinWidgetTypes::HorizontalLayoutGroup;
	parent->Children[name] = std::shared_ptr<UIObject>(this);
}
void UIHorizontalLayoutGroup::OnDirty() {
	int i = 0;
	for (auto& [name, uiObject] : Children) {
		uiObject->LayoutGroupOffset.Y = i * YSpaceBetweenElements;
		uiObject->DirtyInternal();
		++i;
	}
	_drawOrder.clear();
	for (auto& [name, uiObject] : Children) {
		_drawOrder.push_back(uiObject.get());
	}
	std::sort(_drawOrder.begin(), _drawOrder.end(), [](UIObject* lhs, UIObject* rhs) {
		return lhs->Layer() <= rhs->Layer();
	});
}

void UIHorizontalLayoutGroup::Draw() {
	for (auto child : _drawOrder) {
		assert(child);
		child->DrawInternal();
	}
}
