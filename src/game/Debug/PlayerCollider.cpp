#include <Supergoon/ECS/GameObject.h>

#include <Components/Components.hpp>
#include <Components/PlayerComponent.hpp>
#include <Components/PlayerInteractionComponent.hpp>
#include <Debug/PlayerCollider.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#ifdef imgui
#include <SupergoonEngine/imgui/imgui.h>
#endif

using namespace Supergoon;
bool PlayerWidget::ShowPlayerColliderDebugBox = false;
bool PlayerWidget::ShowPlayerExitDebugBox = false;
bool PlayerWidget::ShowPlayerInteractionDebugBox = false;
bool PlayerWidget::ShowInteractionDebugBox = false;
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
	ImGui::Checkbox("Show Interaction Colliders", &ShowInteractionDebugBox);
	// auto playerGo = GameObject::GetGameObjectWithComponents<PlayerComponent>();
	// if (!playerGo.has_value()) {
	// 	ImGui::End();
	// 	return;
	// }
	// auto playerComponent = sgGameObjectGetComponent(playerGo, PlayerComponent);
	// auto locationComponent = sgGameObjectGetComponent(playerGo, LocationComponent);
	// auto playerInteractionComponent = sgGameObjectGetComponent(playerGo, PlayerInteractionComponent);
	// auto gamestateComponent = GameObject::FindComponent<GameState>();
	// assert(gamestateComponent);
	// if (ImGui::CollapsingHeader("Player")) {
	// 	ImGui::DragFloat("Location X", &locationComponent->Location.X, 0.1f);
	// 	ImGui::DragFloat("Location Y", &locationComponent->Location.Y, 0.1f);
	// 	ImGui::DragFloat("BoxOffset X", &playerComponent->Body.X, 0.1f);
	// 	ImGui::DragFloat("BoxOffset Y", &playerComponent->Body.Y, 0.1f);
	// 	ImGui::DragFloat("BoxSize X", &playerComponent->Body.W, 0.1f);
	// 	ImGui::DragFloat("BoxSize Y", &playerComponent->Body.H, 0.1f);
	// 	ImGui::DragFloat("Interaction X", &playerInteractionComponent->InteractionRect.X, 0.1f);
	// 	ImGui::DragFloat("Interaction Y", &playerInteractionComponent->InteractionRect.Y, 0.1f);
	// 	ImGui::DragFloat("Interaction W", &playerInteractionComponent->InteractionRect.W, 0.1f);
	// 	ImGui::DragFloat("Interaction H", &playerInteractionComponent->InteractionRect.H, 0.1f);
	// 	ImGui::BeginDisabled(true);
	// 	ImGui::Checkbox("Interacting", &gamestateComponent->Interacting);
	// 	ImGui::EndDisabled();
	// }
	ImGui::End();
}
