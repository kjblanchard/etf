#include <Supergoon/UI/Panel.hpp>
using namespace Supergoon;
void Panel::Draw() {
	for (auto& [name, uiObject] : Children) {
		uiObject->Draw();
	}
}
