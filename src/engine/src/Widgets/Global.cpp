#include <SupergoonEngine/imgui/imgui.h>

#include <Supergoon/ECS/Components/AnimationComponent.hpp>
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
		Events::PushEvent(Events::BuiltinEvents.ResetGameEvent, 0);
	}
	// TODO this is very ineffieicnt, yes.
	ImGui::Text("Total number of gameobjects %d", GameObject::NumberGameObjects());
	// Show animation component stuff.
	GameObject::ForEach<AnimationComponent>([](GameObject go, AnimationComponent& anim) {
		auto namething = anim.Animation->Filename().c_str();
		if (ImGui::TreeNode(anim.Animation->Filename().c_str())) {
			auto x = std::string("X: ##") + anim.Animation->Filename().c_str();
			auto y = std::string("Y: ##") + anim.Animation->Filename().c_str();
			auto width = std::string("Width: ##") + anim.Animation->Filename().c_str();
			auto height = std::string("Height: ##") + anim.Animation->Filename().c_str();
			ImGui::SliderInt(x.c_str(), &anim.Offset.X, -200, 200);
			ImGui::SliderInt(y.c_str(), &anim.Offset.Y, -200, 200);
			ImGui::SliderInt(width.c_str(), &anim.OverrideDrawSize.X, 0, 400);
			ImGui::SliderInt(height.c_str(), &anim.OverrideDrawSize.Y, 0, 400);
			ImGui::TreePop();
		}
	});
	ImGui::Checkbox("Show Static Debug Colliders", &ShowDebugBoxSolid);
	ImGui::End();
}
