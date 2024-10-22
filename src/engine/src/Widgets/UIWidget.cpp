#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/UI/ImageObject.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/Widgets/UIWidget.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
using namespace Supergoon;

void UIWidget::ShowUiDebugWindow() {
	auto rootPanel = UI::UIInstance;
	if (!rootPanel) {
		return;
	}
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;
	if (!ImGui::Begin("UI", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (ImGui::DragFloat("Root X", &rootPanel->Offset.X, 1.0f)) {
		rootPanel->Dirty = true;
	}
	if (ImGui::DragFloat("Root Y", &rootPanel->Offset.Y, 1.0f)) {
		rootPanel->Dirty = true;
	}

	for (auto &[key, value] : rootPanel->Children) {
		if (value->WidgetType == (int)BuiltinWidgetTypes::Image) {
			auto imageObject = std::dynamic_pointer_cast<ImageObject>(value);
			std::string childX_label = "Child X##" + key;
			std::string childY_label = "Child Y##" + key;
			if (ImGui::CollapsingHeader(key.c_str())) {
				if (ImGui::DragFloat(childX_label.c_str(), &value->Offset.X, 1.0f)) {
					value->Dirty = true;
				}
				if (ImGui::DragFloat(childY_label.c_str(), &value->Offset.Y, 1.0f)) {
					value->Dirty = true;
				}
			}
		}
		// ImGui::End();
	}
	ImGui::End();
}
