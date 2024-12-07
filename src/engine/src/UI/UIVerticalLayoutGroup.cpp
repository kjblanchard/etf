#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Supergoon/pch.hpp>

using namespace Supergoon;

UIVerticalLayoutGroup::UIVerticalLayoutGroup(UIObject* parent, std::string name) : UIObject(parent) {
	WidgetType = (int)BuiltinWidgetTypes::HorizontalLayoutGroup;
	parent->Children[name] = std::shared_ptr<UIObject>(this);
}
void UIVerticalLayoutGroup::OnDirty() {
	int i = 0;
	for (auto& [name, uiObject] : Children) {
		uiObject->LayoutGroupOffset.X = i * XSpaceBetweenElements;
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

void UIVerticalLayoutGroup::Draw() {
	for (auto child : _drawOrder) {
		assert(child);
		child->DrawInternal();
	}
}
