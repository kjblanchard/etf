#include <Components/BattleComponent.hpp>
#include <Components/BattleZoneComponent.hpp>
#include <Components/PlayerComponent.hpp>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/World/Level.hpp>
#include <Supergoon/pch.hpp>
#include <Systems/Battle/BattleZoneSystem.hpp>
using namespace Supergoon;

static bool zonesStarted = false;
static float currentTime = 0.0f;

void updateBattleZones(GameObject go, PlayerComponent &player, LocationComponent &loc) {
  auto state = GameObject::GetGameObjectWithComponents<GameState>();
  auto &stateComponent = state->GetComponent<GameState>();
  assert(state.has_value() && state->HasComponent<GameState>());
  auto battleComponent = GameObject::FindComponent<BattleComponent>();
  if (!battleComponent) {
    return;
  }
  if (battleComponent->EnteringBattle || battleComponent->InBattle) {
    return;
  }
  GameObject::ForEach<BattleZoneComponent>([&go, &stateComponent, &player, &loc, &battleComponent](GameObject, BattleZoneComponent &zone) {
    if (!player.Moving) {
      return;
    }
    auto playerRect = Rectangle(player.Body.X + loc.Location.X,
                                player.Body.Y + loc.Location.Y,
                                player.Body.W,
                                player.Body.H);
    if (playerRect.IsOverlap(zone.BoundingBox)) {
      currentTime += stateComponent.DeltaTime;
      if (currentTime < zone.EncounterTime) {
        return;
      }
      // TODO, why is this happening here?  Should probably be a function to enter the battle with?  We are setting follow target etc here, and we do it in the battle widget.
      assert(go.HasComponent<AnimationComponent>());
      auto &anim = go.GetComponent<AnimationComponent>();
      stateComponent.PlayerLoadLocation.X = loc.Location.X;
      stateComponent.PlayerLoadLocation.Y = loc.Location.Y;
      stateComponent.PlayerLoadLevel = Level::GetName();
      stateComponent.CameraFollowTarget = false;
      battleComponent->EnteringBattle = true;
      stateComponent.PlayerLoadDirection = (int)player.Direction;
      Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("battle1"));
      anim.Playing = false;
      currentTime = 0;
      // TODO Disable the screen transition screen if you enter a battle quickly.  Should probably just make a event for this that UI listens to.
      auto displayTransitionUI = UI::UIInstance->GetChildByName("screen");
      if (displayTransitionUI && displayTransitionUI->Visible()) {
        displayTransitionUI->SetVisible(false);
      }
      return;
    }
  });
}
void Supergoon::StartBattleZones() {
  if (zonesStarted) {
    return;
  }
}

void Supergoon::UpdateBattleZones() {
  GameObject::ForEach<PlayerComponent, LocationComponent>(updateBattleZones);
}
