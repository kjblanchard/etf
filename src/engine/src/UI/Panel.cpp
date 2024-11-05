#include <Supergoon/UI/Panel.hpp>
using namespace Supergoon;
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
