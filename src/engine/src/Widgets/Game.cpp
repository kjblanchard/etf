#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/Events.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Widgets/Game.hpp>
namespace Supergoon {

bool GameWidget::_isFocusedLastFrame = true;

void GameWidget::ShowGameDebugWindow() {
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

	if (!ImGui::Begin("Game", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	bool isFocused = ImGui::IsWindowFocused();
	if (_isFocusedLastFrame != isFocused) {
		Events::PushEvent(Events::BuiltinEvents.ImGuiFocusedEvent, isFocused);
		_isFocusedLastFrame = isFocused;
	}
	auto graphics = Graphics::Instance();

	ImVec2 imguiWindowSize = ImGui::GetContentRegionAvail();
	// Step 1: Calculate the integer scaling factor
	int scaleX = imguiWindowSize.x / graphics.LogicalWidth();	// Integer division for X axis
	int scaleY = imguiWindowSize.y / graphics.LogicalHeight();	// Integer division for Y axis

	// Choose the smaller scale factor to maintain aspect ratio
	int scale = (scaleX < scaleY) ? scaleX : scaleY;

	// Step 2: Calculate the scaled size (what SDL would render to the screen)
	int scaledWidth = graphics.LogicalWidth() * scale;
	int scaledHeight = graphics.LogicalHeight() * scale;

	// Step 3: Render the SDL_Texture in ImGui with the same scaling
	ImVec2 imguiSize(scaledWidth, scaledHeight);  // Use the scaled size
	auto tex = graphics.ImGuiTexture();
	// ImGui::Image(tex, ImVec2(graphics.WindowWidth(), graphics.WindowHeight()));
	ImGui::Image(tex, imguiSize);
	ImGui::End();
}
}  // namespace Supergoon
