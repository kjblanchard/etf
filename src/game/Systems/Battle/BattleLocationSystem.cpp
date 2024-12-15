#include <Components/BattleLocationComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Systems/Battle/BattleLocationSystem.hpp>
using namespace Supergoon;

void loadBattlers(GameObject, BattleLocationComponent &battleLocation) {
  if (battleLocation.BattleLocationId != 4 && battleLocation.BattleLocationId != 1) {
    return;
  }
  auto go = new GameObject();
  //  auto battleLocationComponent = BattleLocationComponent();
  auto battleLocationLoc = LocationComponent();
  auto battlerAnimation = AnimationComponent();
  auto battlerComponent = BattlerComponent();
  battlerComponent.CurrentATB = 0;
  battlerComponent.FullATB = 3;
  if (battleLocation.BattleLocationId == 1) {
    battlerAnimation.AnimationName = "player1Battler";
    battlerComponent.Id = 1;
    battlerComponent.IsPlayer = true;
  } else {
    battlerAnimation.AnimationName = "blackBird";
    battlerComponent.Id = 4;
    battlerComponent.IsPlayer = false;
  }
  battlerAnimation.Offset = {0, 0};
  battlerAnimation.AnimationSpeed = 1.0;
  battleLocationLoc.Location.X = battleLocation.Location.X;
  battleLocationLoc.Location.Y = battleLocation.Location.Y;
  go->AddComponent<AnimationComponent>(battlerAnimation);
  go->AddComponent<BattlerComponent>(battlerComponent);
  go->AddComponent<LocationComponent>(battleLocationLoc);
  Events::PushEvent(Events::BuiltinEvents.GameObjectAdd, true, (void *)go);
}
void startBattlers(GameObject, BattlerComponent &battler, AnimationComponent &anim) {
  if (battler.Id == 1) {
    anim.Animation->PlayAnimation("idle1");
    anim.Offset.X = -21;
    anim.Offset.Y = -41;
    anim.OverrideDrawSize = {72, 72};
    anim.Playing = true;
  } else {
    anim.Offset.X = -15;
    anim.Offset.Y = -26;
    anim.Animation->PlayAnimation("idleR");
    anim.Playing = true;
  }
}

void Supergoon::LoadBattlers() {
  GameObject::ForEach<BattleLocationComponent>(loadBattlers);
}

void Supergoon::StartBattlers() {
  GameObject::ForEach<BattlerComponent, AnimationComponent>(startBattlers);
}
