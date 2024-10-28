#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/Events.hpp>
#include <Supergoon/Widgets/Global.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
using namespace Supergoon;

bool GlobalWidget::ShowDebugBoxSolid = false;

void GlobalWidget::ShowGlobalDebugWindow() {
	auto window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;
	if (!ImGui::Begin("Global", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	if (ImGui::Button("Reset##globalreset")) {
		for (size_t i = 0; i < 15; i++) {
			Events::PushEvent(Events::BuiltinEvents.ResetGameEvent, 0);
			/* code */
		}
	}
	ImGui::Checkbox("Show Static Debug Colliders", &ShowDebugBoxSolid);
	ImGui::End();
}
