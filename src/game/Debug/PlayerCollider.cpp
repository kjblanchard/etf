#include <Components/PlayerComponent.hpp>
#include <Debug/PlayerCollider.hpp>
#include <Supergoon/Supergoon.hpp>

using namespace Supergoon;
bool PlayerWidget::ShowPlayerColliderDebugBox = false;
bool PlayerWidget::ShowPlayerExitDebugBox = false;
bool PlayerWidget::ShowPlayerInteractionDebugBox = false;
void PlayerWidget::ShowPlayerColliderWindow() {
	ImGuiWindowFlags window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;

	if (!ImGui::Begin("Player Debug", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	ImGui::Checkbox("Show Player Debug Colliders", &ShowPlayerColliderDebugBox);
	ImGui::Checkbox("Show Player Exit Colliders", &ShowPlayerExitDebugBox);
	ImGui::Checkbox("Show Player Interaction Colliders", &ShowPlayerInteractionDebugBox);
	auto playerGo = GameObject::GetGameObjectWithComponents<PlayerComponent>();
	if (!playerGo.has_value()) {
		ImGui::End();
		return;
	}
	auto& playerComponent = playerGo->GetComponent<PlayerComponent>();
	auto& locationComponent = playerGo->GetComponent<LocationComponent>();
	if (ImGui::CollapsingHeader("Player")) {
		ImGui::DragFloat("Location X", &locationComponent.Location.X, 0.1f);
		ImGui::DragFloat("Location Y", &locationComponent.Location.Y, 0.1f);
		ImGui::DragFloat("BoxOffset X", &playerComponent.Body.X, 0.1f);
		ImGui::DragFloat("BoxOffset Y", &playerComponent.Body.Y, 0.1f);
		ImGui::DragFloat("BoxSize X", &playerComponent.Body.W, 0.1f);
		ImGui::DragFloat("BoxSize Y", &playerComponent.Body.H, 0.1f);
		ImGui::DragFloat("Interaction X", &playerComponent.InteractionRect.X, 0.1f);
		ImGui::DragFloat("Interaction Y", &playerComponent.InteractionRect.Y, 0.1f);
		ImGui::DragFloat("Interaction W", &playerComponent.InteractionRect.W, 0.1f);
		ImGui::DragFloat("Interaction H", &playerComponent.InteractionRect.H, 0.1f);
	}
	ImGui::End();
}
