#include <Supergoon/UI/Panel.hpp>
#include <algorithm>
#include <cassert>
using namespace Supergoon;
Panel::Panel() : UIObject() {
}
Panel::Panel(Panel* parent, std::string name) : UIObject(parent) {
	WidgetType = (int)BuiltinWidgetTypes::Panel;
	parent->Children[name] = std::shared_ptr<UIObject>(this);
}
void Panel::OnDirty() {
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	for (auto& [name, uiObject] : Children) {
		uiObject->OnDirty();
	}
	_drawOrder.clear();
	for (auto& [name, uiObject] : Children) {
		_drawOrder.push_back(uiObject.get());
	}
	std::sort(_drawOrder.begin(), _drawOrder.end(), [](UIObject* lhs, UIObject* rhs) {
		return lhs->Layer() <= rhs->Layer();
	});
}
void Panel::Update() {
	for (auto& [name, uiObject] : Children) {
		if (!uiObject) {
			continue;
		}
		uiObject->UpdateInternal();
	}
}
void Panel::Draw() {
	// for (auto& [name, uiObject] : Children) {
	// 	uiObject->DrawInternal();
	// }
	for (auto child : _drawOrder) {
		assert(child);
		child->DrawInternal();
	}
}
