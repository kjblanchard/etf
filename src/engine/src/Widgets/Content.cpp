#include <SupergoonEngine/imgui.h>

#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Widgets/Content.hpp>
namespace Supergoon {

void ContentWidget::ShowContentDebugWindow() {
	static bool p_open = true;
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = false;
	static bool no_move = false;
	static bool no_resize = false;
	static bool no_collapse = false;
	static bool no_close = false;
	static bool no_nav = false;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool no_docking = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar) window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar) window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu) window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move) window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize) window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse) window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav) window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background) window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front) window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (no_docking) window_flags |= ImGuiWindowFlags_NoDocking;
	if (unsaved_document) window_flags |= ImGuiWindowFlags_UnsavedDocument;
	if (no_close) p_open = NULL;  // Don't pass our bool* to Begin

	if (!ImGui::Begin("Content", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::Text("Number of loaded content is %d", ContentRegistry::_loadedContent.size());
	ImGui::End();
}
}  // namespace Supergoon
