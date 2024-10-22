#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/Events.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Widgets/Game.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
namespace Supergoon {

bool GameWidget::_isFocusedLastFrame = true;

void GameWidget::ShowGameDebugWindow() {
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;
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
