#include <Supergoon/UI/Panel.hpp>
using namespace Supergoon;
void Panel::OnDirty() {
	for (auto& [name, uiObject] : Children) {
		uiObject->OnDirty();
	}
}
void Panel::Draw() {
	for (auto& [name, uiObject] : Children) {
		uiObject->Draw();
	}
}
