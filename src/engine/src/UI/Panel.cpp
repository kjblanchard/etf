#include <Supergoon/UI/Panel.hpp>
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
	for (auto& [name, uiObject] : Children) {
		uiObject->DrawInternal();
	}
}
