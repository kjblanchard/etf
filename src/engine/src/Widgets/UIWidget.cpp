#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIImage.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/Widgets/UIWidget.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
using namespace Supergoon;
static bool shouldDrawDebugBox = false;
void UIWidget::DrawPanel(Panel *panel, std::string panelName) {
	if (ImGui::TreeNode((panelName + "- panel").c_str())) {
		auto panelOffsetLabel = "Offset X ##" + panelName;
		auto panelOffsetYLabel = "Offset Y ##" + panelName;
		auto panelTransparencyLabel = "Transparency ##" + panelName;
		auto visibleLable = "Visible ##" + panelName;
		if (ImGui::Checkbox(visibleLable.c_str(), &panel->_visible)) {
			panel->Dirty = true;
		}
		if (ImGui::DragFloat(panelOffsetLabel.c_str(), &panel->Offset.X, 1.0f)) {
			panel->Dirty = true;
		}
		if (ImGui::DragFloat(panelOffsetYLabel.c_str(), &panel->Offset.Y, 1.0f)) {
			panel->Dirty = true;
		}
		if (ImGui::DragInt(panelTransparencyLabel.c_str(), panel->AlphaHandle(), 1, 0, 255, "%d")) {
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
					if (ImGui::DragInt(transLabel.c_str(), imageObject->AlphaHandle(), 1, 0, 255, "%d", ImGuiSliderFlags_WrapAround)) {
						value->Dirty = true;
					}
					ImGui::TreePop();
				}
			} else if (value->WidgetType == (int)BuiltinWidgetTypes::Text) {
				assert((UIText *)value.get());
				auto textUIObject = (UIText *)value.get();
				std::string childX_label = "Offset X##" + key;
				std::string childY_label = "Offset Y##" + key;
				std::string childW_label = "Width##" + key;
				std::string childH_label = "Height##" + key;
				std::string childXBounds = "TextBoundsX##" + key;
				std::string childYBounds = "TextBoundsY##" + key;
				std::string childWordWrapLabel = "WordWrap##" + key;
				std::string childCenterLabel = "Center##" + key;
				std::string childLettersToDraw = "Letters To Draw##" + key;
				std::string childDebugBoxCheckbox = "DebugBox##" + key;
				if (ImGui::TreeNode((key + "- text").c_str())) {
					if (ImGui::DragFloat(childX_label.c_str(), &value->Offset.X, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childY_label.c_str(), &value->Offset.Y, 1.0f)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childW_label.c_str(), &value->Bounds.W, 1.0f, 0.0f, FLT_MAX)) {
						value->Dirty = true;
					}
					if (ImGui::DragFloat(childH_label.c_str(), &value->Bounds.H, 1.0f, 0.0f, FLT_MAX)) {
						value->Dirty = true;
					}
					if (ImGui::Checkbox(childWordWrapLabel.c_str(), &textUIObject->WordWrap)) {
						value->Dirty = true;
					};
					ImGui::SameLine();
					if (ImGui::Checkbox(childCenterLabel.c_str(), &textUIObject->CenterText)) {
						value->Dirty = true;
					};
					ImGui::SameLine();
					ImGui::Checkbox(childDebugBoxCheckbox.c_str(), &shouldDrawDebugBox);

					if(shouldDrawDebugBox) {
						Graphics::Instance()->DrawRect(textUIObject->Bounds, Color(255, 0, 0, 255));

					}
					if (ImGui::DragInt(childLettersToDraw.c_str(), &textUIObject->currentLetters, 1, 0, textUIObject->TextPtr->_text.length())) {
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
