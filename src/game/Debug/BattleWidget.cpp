#include <Components/BattleComponent.hpp>
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
  ImGui::BeginDisabled(true);
  ImGui::Text("Current Battle State %d: %s", (int)battleComponent->CurrentBattleState, GetBattleStateText(battleComponent->CurrentBattleState));
  ImGui::EndDisabled();
  ImGui::End();
}
