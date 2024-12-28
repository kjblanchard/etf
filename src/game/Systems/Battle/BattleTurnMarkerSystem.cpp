#include <Components/BattlerComponent.hpp>
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/KeepAliveComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Systems/Battle/BattleTurnMarkerSystem.hpp>
#include <Utilities/Events.hpp>
using namespace Supergoon;

static GameObject *turnMarkerGameObject = nullptr;

void Supergoon::InitializeBattleTurnMarkerSystem() {
  Events::RegisterEventHandler(EscapeTheFateEvents.PlayerBattlerTurnBegin, [](int battler, void *, void *) {
    GameObject::ForEach<BattlerComponent>([battler](GameObject go, BattlerComponent &battlerComp) {
      if (battlerComp.IsPlayer && battler == battlerComp.Id) {
        assert(go.HasComponent<AnimationComponent>() && go.HasComponent<LocationComponent>() && turnMarkerGameObject && turnMarkerGameObject->HasComponent<LocationComponent>() && turnMarkerGameObject->HasComponent<AnimationComponent>() && "Bad components");
        auto &anim = go.GetComponent<AnimationComponent>();
        auto &loc = go.GetComponent<LocationComponent>();
        auto &turnMarkerLoc = turnMarkerGameObject->GetComponent<LocationComponent>();
        auto &turnMarkerAnim = turnMarkerGameObject->GetComponent<AnimationComponent>();
        turnMarkerAnim.Visible = true;
        turnMarkerAnim.Playing = true;
        turnMarkerAnim.Animation->PlayAnimation("idle");
        turnMarkerLoc.Location.X = loc.Location.X + anim.Offset.X;
        turnMarkerLoc.Location.Y = loc.Location.Y + anim.Offset.Y - turnMarkerAnim.AnimationImage->Height();
      }
    });
    Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int battler, void *, void *) {
      assert(turnMarkerGameObject && turnMarkerGameObject->HasComponent<LocationComponent>() && turnMarkerGameObject->HasComponent<AnimationComponent>() && "Bad components");
      auto &turnMarkerAnim = turnMarkerGameObject->GetComponent<AnimationComponent>();
      turnMarkerAnim.Visible = false;
      turnMarkerAnim.Playing = false;
    });
  });
}
void Supergoon::LoadBattleTurnMarkerSystem() {
  if (turnMarkerGameObject) {
    return;
  }
  GameObject *go = new GameObject();
  auto anim = AnimationComponent();
  anim.AnimationName = "battleTurnMarker";
  anim.Offset = {29, 29};
  anim.Playing = true;
  anim.AnimationSpeed = 1.0f;
  anim.Layer = 1;
  anim.Visible = false;
  auto loc = LocationComponent();
  auto keep = KeepAliveComponent();
  go->AddComponent<AnimationComponent>(anim);
  go->AddComponent<LocationComponent>(loc);
  go->AddComponent<KeepAliveComponent>(keep);
  turnMarkerGameObject = go;
  Events::PushEvent(Events::BuiltinEvents.GameObjectAdd, 0, (void *)go);
}
