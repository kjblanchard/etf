#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/UI/ImageObject.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/Widgets/UIWidget.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
using namespace Supergoon;
static void DrawPanel(Panel *panel, std::string panelName) {
	if (ImGui::TreeNode((panelName + "- panel").c_str())) {
		auto panelOffsetLabel = "Offset X ##" + panelName;
		auto panelOffsetYLabel = "Offset Y ##" + panelName;
		if (ImGui::DragFloat(panelOffsetLabel.c_str(), &panel->Offset.X, 1.0f)) {
			panel->Dirty = true;
		}
		if (ImGui::DragFloat(panelOffsetYLabel.c_str(), &panel->Offset.Y, 1.0f)) {
			panel->Dirty = true;
		}
		for (auto &[key, value] : panel->Children) {
			if (value->WidgetType == (int)BuiltinWidgetTypes::Panel) {
				assert((Panel *)value.get());
				DrawPanel((Panel *)value.get(), key);
			} else if (value->WidgetType == (int)BuiltinWidgetTypes::Image) {
				auto imageObject = std::dynamic_pointer_cast<ImageObject>(value);
				std::string childX_label = "Offset X##" + key;
				std::string childY_label = "Offset Y##" + key;
				std::string childWLabel = "Child W##" + key;
				std::string childHLabel = "Child H##" + key;
				std::string transLabel = "Child Transparency##" + key;
				if (ImGui::TreeNode((key + "- image").c_str())) {
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
		ImGui::TreePop();
	}
}

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
		// We will need to loop over all panels animators
		for (auto &&[key, value] : UI::UIInstance->Children) {
			for (auto &&animator : value->Animators) {
				if (ImGui::TreeNode(animator->Name.c_str())) {
					auto name = std::string("Play ##") + animator->Name;
					auto valueText = animator->IsPlaying() ? "True" : "False";
					// auto playingText = std::string("Is Playing ") + valueText + "## " + animator->Name;
					auto playingText = std::string("Is Playing ") + valueText;
					ImGui::Text("%s", playingText.c_str());
					ImGui::SameLine();
					auto percentText = std::string("Percent: ") + std::to_string(animator->SequenceToPlay->Percent());
					ImGui::Text("%s", percentText.c_str());

					if (ImGui::Button(name.c_str())) {
						animator->Play();
					}
					ImGui::SameLine();
					auto stop = std::string("Stop ##") + animator->Name;
					if (ImGui::Button(stop.c_str())) {
						animator->Stop();
					}
					ImGui::SameLine();
					auto pause = std::string("Pause ##") + animator->Name;
					if (ImGui::Button(pause.c_str())) {
						animator->Pause();
					}
					ImGui::TreePop();
				}
			}
		}
	}
	if (ImGui::CollapsingHeader("UI Elements")) {
		DrawPanel(rootPanel, "Root");
	}
	ImGui::End();
}
