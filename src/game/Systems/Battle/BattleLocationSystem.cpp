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
  battlerComponent.CurrentBattleLocation = battleLocation.BattleLocationId;
  if (battleLocation.BattleLocationId == 1) {
    battlerAnimation.AnimationName = "player1Battler";
    battlerComponent.Id = 1;
    battlerComponent.IsPlayer = true;
    battlerComponent.FullATB = 3;
    battlerComponent.Stat.Name = "Kevin";
    battlerComponent.Stat.HP = 10;
    battlerComponent.Stat.MaxHP = 10;
    battlerComponent.Stat.Str = 1;
    // TODO we need to script this somehow,
    battlerAnimation.OnAnimationEnd = [](AsepriteAnimation *anim, std::string animEnding) {
      if (animEnding == "slash2" || animEnding == "damage1") {
        anim->PlayAnimation("idle1");
      }
    };
  } else {
    battlerAnimation.AnimationName = "blackBird";
    battlerComponent.Stat.HP = 2;
    battlerComponent.Stat.MaxHP = 2;
    battlerComponent.Id = 4;
    battlerComponent.FullATB = 5;
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
