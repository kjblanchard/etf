#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Components/PlayerComponent.hpp>
#include <Debug/BattleWidget.hpp>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <Supergoon/World/Level.hpp>
#include <Supergoon/pch.hpp>
using namespace Supergoon;
using namespace std;

void BattleWidget::ShowBattleWidget() {
  ImGuiWindowFlags window_flags = Widgets::GetDefaultWindowFlags();
  bool p_open;
  //  TODO, a lot of this shouldn't be on the gamestate component, probably make a battle component here, gamestate is just for things between scenes.
  auto gamestateComponent = GameObject::FindComponent<GameState>();
  auto battleComponent = GameObject::FindComponent<BattleComponent>();
  // assert(gamestateComponent && battleComponent && "Could not find components for battle widget");
  if (!ImGui::Begin("Battle", &p_open, window_flags) || !gamestateComponent || !battleComponent) {
    // Early out if the window is collapsed, as an optimization.
    ImGui::End();
    return;
  }
  if (ImGui::Button("Start Battle")) {
    auto go = GameObject::GetGameObjectWithComponents<PlayerComponent>();
    assert(go.has_value() && go->HasComponent<LocationComponent>() && go->HasComponent<AnimationComponent>());
    auto &anim = go->GetComponent<AnimationComponent>();
    auto &player = go->GetComponent<PlayerComponent>();
    auto &loc = go->GetComponent<LocationComponent>();
    gamestateComponent->PlayerLoadLocation.X = loc.Location.X;
    gamestateComponent->PlayerLoadLocation.Y = loc.Location.Y;
    gamestateComponent->PlayerLoadLevel = Level::GetName();
    gamestateComponent->CameraFollowTarget = false;
    battleComponent->EnteringBattle = true;
    Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("battle1"));
    anim.Playing = false;
    gamestateComponent->PlayerLoadDirection = (int)player.Direction;
    gamestateComponent->CameraFollowTarget = false;
  }
  for (auto battler : battleComponent->Battlers) {
    auto playerDisplayName = battler->Stat.Name + string("## ") + to_string(battler->Id);
    if (ImGui::TreeNode(playerDisplayName.c_str())) {
      ImGui::Text("ID: %d", battler->Id);
      ImGui::Text("ATB: %f", battler->CurrentATB);
      ImGui::SameLine();
      ImGui::Text(" / : %f", battler->FullATB);
      ImGui::Text("Battle Location %d", battler->CurrentBattleLocation);
      ImGui::Text("Player:  %d", battler->IsPlayer);
      ImGui::TreePop();
      auto statName = "Stats ##" + to_string(battler->Id);
      if (ImGui::TreeNode(statName.c_str())) {
        ImGui::Text("HP: %d", battler->Stat.HP);
        ImGui::Text("Max HP: %d", battler->Stat.MaxHP);
        ImGui::Text("Str: %d", battler->Stat.Str);
        ImGui::Text("Def: %d", battler->Stat.Def);
        ImGui::Text("Spd: %d", battler->Stat.Spd);
        ImGui::TreePop();
      }
    }
  }

  ImGui::BeginDisabled(true);
  ImGui::Text("Current Battle State %d: %s", (int)battleComponent->CurrentBattleState, GetBattleStateText(battleComponent->CurrentBattleState));
  ImGui::EndDisabled();
  ImGui::End();
}
