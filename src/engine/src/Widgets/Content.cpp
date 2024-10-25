#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Widgets/Content.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
namespace Supergoon {

void ContentWidget::ShowContentDebugWindow() {
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;

	if (!ImGui::Begin("Content", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::Text("Number of loaded content is %zu", ContentRegistry::_loadedContent.size());
	if (ImGui::TreeNode("Contents")) {
		for (const auto& [key, value] : ContentRegistry::_loadedContent) {
			if (ImGui::TreeNode(key.c_str())) {
				ImGui::Text("Size:");
				ImGui::SameLine();
				ImGui::Text("%lu", sizeof(*value.get()));
				ImGui::Text("Type:");
				ImGui::SameLine();
				ImGui::Text("%s", value->Type().c_str());
				ImGui::Text("Path:");
				ImGui::SameLine();
				ImGui::Text("%s", value->Filepath().c_str());
				ImGui::Text("RefCount:");
				ImGui::SameLine();
				ImGui::Text("%ld", value.use_count());
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
}  // namespace Supergoon
