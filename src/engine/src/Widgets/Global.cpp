#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/ECS/Gameobject.hpp>
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
	// TODO this is very ineffieicnt, yes.
	ImGui::Text("Total number of gameobjects %d", GameObject::NumberGameObjects());
	ImGui::Checkbox("Show Static Debug Colliders", &ShowDebugBoxSolid);
	ImGui::End();
}
