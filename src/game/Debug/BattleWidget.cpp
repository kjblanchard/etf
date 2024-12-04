#include <Components/PlayerComponent.hpp>
#include <Debug/BattleWidget.hpp>
#include <Supergoon/Supergoon.hpp>
#include <optional>
using namespace Supergoon;

void BattleWidget::ShowBattleWidget() {
	ImGuiWindowFlags window_flags = Widgets::GetDefaultWindowFlags();
	bool p_open;

	if (!ImGui::Begin("Battle", &p_open, window_flags)) {
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}
	//  TODO, a lot of this shouldn't be on the gamestate component, probably make a battle component here, gamestate is just for things between scenes.
	auto gamestateComponent = GameObject::FindComponent<GameState>();
	assert(gamestateComponent);
	if (ImGui::Button("Start Battle")) {
		auto go = GameObject::GetGameObjectWithComponents<PlayerComponent>();
		assert(go.has_value() && go->HasComponent<LocationComponent>() && go->HasComponent<AnimationComponent>());
		auto& anim = go->GetComponent<AnimationComponent>();
		auto& player = go->GetComponent<PlayerComponent>();
		auto& loc = go->GetComponent<LocationComponent>();
		gamestateComponent->PlayerLoadLocation.X = loc.Location.X;
		gamestateComponent->PlayerLoadLocation.Y = loc.Location.Y;
		gamestateComponent->PlayerLoadLevel = Level::GetName();
		gamestateComponent->CameraFollowTarget = false;
		gamestateComponent->EnteringBattle = true;
		Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void*)strdup("battle1"));
		anim.Playing = false;
		gamestateComponent->PlayerLoadDirection = (int)player.Direction;
		gamestateComponent->CameraFollowTarget = false;
		gamestateComponent->EnteringBattle = true;
	}
	ImGui::BeginDisabled(true);
	ImGui::Checkbox("In battle", &gamestateComponent->InBattle);
	ImGui::Checkbox("Entering Battle", &gamestateComponent->EnteringBattle);
	ImGui::Checkbox("Exiting Battle", &gamestateComponent->ExitingBattle);
	ImGui::Text("Current Battler: %d", gamestateComponent->BattleData.CurrentBattler);
	ImGui::EndDisabled();
	ImGui::End();
}
