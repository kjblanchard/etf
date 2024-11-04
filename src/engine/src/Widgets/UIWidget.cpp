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

	if (ImGui::CollapsingHeader("UI Animators")) {
		for (auto &&animator : UI::Animators) {
		}
	}
	if (ImGui::CollapsingHeader("UI Elements")) {
		for (auto &[key, value] : rootPanel->Children) {
			if (value->WidgetType == (int)BuiltinWidgetTypes::Image) {
				auto imageObject = std::dynamic_pointer_cast<ImageObject>(value);
				std::string childX_label = "Offset X##" + key;
				std::string childY_label = "Offset Y##" + key;
				std::string childWLabel = "Child W##" + key;
				std::string childHLabel = "Child H##" + key;
				std::string transLabel = "Child Transparency##" + key;
				if (ImGui::TreeNode(key.c_str())) {
					if (ImGui::DragFloat(childX_label.c_str(), &value->Offset.X, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childY_label.c_str(), &imageObject->Offset.Y, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childWLabel.c_str(), &imageObject->Bounds.W, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childHLabel.c_str(), &imageObject->Bounds.H, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragInt(transLabel.c_str(), &imageObject->Transparency, 1, 0, 255, "%d", ImGuiSliderFlags_WrapAround)) {
						value->Dirty = true;
					}
					ImGui::TreePop();
				}
			}
			// ImGui::End();
		}
	}
	ImGui::End();
}
