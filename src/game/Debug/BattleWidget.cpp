#include <Debug/BattleWidget.hpp>
#include <Supergoon/Supergoon.hpp>
using namespace Supergoon;

void BattleWidget::ShowBattleWidget() {
	ImGuiWindowFlags window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;

	if (!ImGui::Begin("Battle", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	auto gamestateComponent = GameObject::FindComponent<GameState>();
	assert(gamestateComponent);
	ImGui::BeginDisabled(true);
	ImGui::Checkbox("In battle", &gamestateComponent->InBattle);
	ImGui::Checkbox("Entering Battle", &gamestateComponent->EnteringBattle);
	ImGui::Checkbox("Exiting Battle", &gamestateComponent->ExitingBattle);
	ImGui::Text("Current Battler: %d", gamestateComponent->BattleData.CurrentBattler);
	ImGui::EndDisabled();
	ImGui::End();
}
