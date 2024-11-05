#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Widgets/ContentWidget.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <vector>
namespace Supergoon {

void ContentWidget::ShowContentDebugWindow() {
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;

	if (!ImGui::Begin("Content", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (ImGui::Button("Clear StaleContent")) {
		ContentRegistry::ClearStaleContent(true);
	}
	ImGui::Text("Number of loaded content is %zu", ContentRegistry::_loadedContent.size());
	// Create a temporary vector to hold all the content and sort it properly, for display purposes..
	std::unordered_map<std::string, std::vector<std::weak_ptr<Content>>> typedContents;
	for (const auto& [key, value] : ContentRegistry::_loadedContent) {
		typedContents[value->Type()].push_back(value);
	}
	if (ImGui::TreeNode("Contents")) {
		for (const auto& [typeName, vectorOfValues] : typedContents) {
			if (ImGui::TreeNode(typeName.c_str())) {
				for (auto i : vectorOfValues) {
					auto item = i.lock();
					if (ImGui::TreeNode(item->Filepath().c_str())) {
						ImGui::Text("Path:");
						ImGui::SameLine();
						ImGui::Text("%s", item->Filepath().c_str());
						ImGui::Text("RefCount:");
						ImGui::SameLine();
						// Refcount here should be reduced by 1 for the lock, and another for the spot in the content registry for a more accurate number for viewing.
						ImGui::Text("%ld", item.use_count() - 2);
						ImGui::TreePop();
					}
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	ImGui::End();
}
}  // namespace Supergoon
