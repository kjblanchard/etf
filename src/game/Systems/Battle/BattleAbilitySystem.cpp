#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleAbility.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Coroutine.h>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/KeepAliveComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <Systems/Battle/BattleAbilitySystem.hpp>
#include <Utilities/Events.hpp>
#include <cassert>
#include <memory>
using namespace Supergoon;
using namespace std;
static GameState *gamestate = nullptr;
static BattleComponent *battleComponent = nullptr;
static const int _numAbilities = 1;
static BattleAbility _abilities[] = {
    {0, DamageTypes::Physical, 0, 1.0f, "slash2", "slash1", "sword1", nullptr}};
static shared_ptr<Sfx> slashSfx = nullptr;

static int calculatePhysicalDamage(BattlerComponent *target, BattlerComponent *attacker, BattleAbility *ability) {
  auto damage = attacker->Stat.Str * ability->Multiplier;
  damage -= target->Stat.Def;
  return damage;
}

static int calculateDamage(BattlerComponent *target, BattlerComponent *attacker, BattleAbility *ability) {
  switch (ability->DamageType) {
  case DamageTypes::Physical:
    return calculatePhysicalDamage(target, attacker, ability);
  default:
    sgLogWarn("Trying to calculate damage for a type not implemented yet!");
    return 0;
  }
}

static void handleAbilityUsed(int, void *abilityArgs, void *) {
  if (!gamestate) {
    gamestate = GameObject::FindComponent<GameState>();
  }
  if (!battleComponent) {
    battleComponent = GameObject::FindComponent<BattleComponent>();
  }
  assert((BattleCommandArgs *)abilityArgs && "Bad battle command arg event passed");
  auto abilityArg = (BattleCommandArgs *)abilityArgs;
  if (!battleComponent->InBattle || !abilityArg || !gamestate) {
    return;
  }
  // Calculate the damage
  assert(abilityArg->AttackingBattler.HasComponent<BattlerComponent>() && abilityArg->TargetBattler.HasComponent<BattlerComponent>() && "Gameobjects passed to ability system do not have battler components");
  auto attackingBattlerComponent = abilityArg->AttackingBattler.GetComponent<BattlerComponent>();
  auto targetBattlerComponent = abilityArg->TargetBattler.GetComponent<BattlerComponent>();
  assert(abilityArg->AbilityId < _numAbilities && _abilities[abilityArg->AbilityId].ID == abilityArg->AbilityId && "Ability out of array bounds, or ability is not configured properly");
  auto ability = &_abilities[abilityArg->AbilityId];
  auto damage = calculateDamage(&targetBattlerComponent, &attackingBattlerComponent, ability);
  // Start any player animations of using this ability
  assert(abilityArg->AttackingBattler.HasComponent<AnimationComponent>() && "Battler doesn't have a animation component somehow");
  auto animComp = abilityArg->AttackingBattler.GetComponent<AnimationComponent>();
  animComp.Animation->PlayAnimation(ability->CharAnimationName);
  // Start any ability animations of using this ability
  assert(abilityArg->TargetBattler.HasComponent<LocationComponent>() && abilityArg->TargetBattler.HasComponent<AnimationComponent>() && "Target battler doesn't have proper components");
  auto targetAnimComponent = abilityArg->TargetBattler.GetComponent<AnimationComponent>();
  auto targetLocationComponent = abilityArg->TargetBattler.GetComponent<LocationComponent>();
  assert(_abilities[abilityArg->AbilityId].AnimationGameObject->HasComponent<AnimationComponent>() && _abilities[abilityArg->AbilityId].AnimationGameObject->HasComponent<LocationComponent>());
  auto &abilityLocation = _abilities[abilityArg->AbilityId].AnimationGameObject->GetComponent<LocationComponent>();
  auto &abilityAnim = _abilities[abilityArg->AbilityId].AnimationGameObject->GetComponent<AnimationComponent>();
  auto abilityAnimLocationX = targetLocationComponent.Location.X + targetAnimComponent.Offset.X;
  auto abilityAnimLocationY = targetLocationComponent.Location.Y + targetAnimComponent.Offset.Y;
  abilityLocation.Location.X = abilityAnimLocationX;
  abilityLocation.Location.Y = abilityAnimLocationY;
  abilityAnim.Offset = {-27, -27};
  abilityAnim.Animation->Load();
  abilityAnim.OverrideDrawSize.X = 128;
  abilityAnim.OverrideDrawSize.Y = 128;
  auto damageCo = sgAddCoroutine(
      0.5, [](void *damage, void *args) {
        assert((BattleCommandArgs *)args && "Could not convert!");

        Events::PushEvent(EscapeTheFateEvents.BattleDamageEvent, (intptr_t)damage, args);
      },
      (void *)damage, (void *)abilityArgs);
  auto slashCo = sgAddCoroutine(
      0.01, [](void *name, void *animComponent) {
        assert((const char *)name && (AnimationComponent *)animComponent && "Could not convert name from void* for some reason");
        auto abilityAnim = (AnimationComponent *)animComponent;
        auto animName = (const char *)name;
        abilityAnim->Visible = true;
        abilityAnim->Playing = true;
        abilityAnim->Animation->PlayAnimation(animName);
        abilityAnim->Animation->OnAnimationEnd = [animName, abilityAnim](AsepriteAnimation *, std::string animEnding) {
          if (animEnding == animName) {
            abilityAnim->Visible = false;
          }
        };
      },
      (void *)"slash1", (void *)&abilityAnim);
  // Play SFX for this
  auto co = sgAddCoroutine(
      0.25, [](void *name, void *) {
        assert((const char *)name && "Could not convert name from void* for some reason");
        Sound::Instance()->PlaySfxOneShot((const char *)name);
      },
      (void *)_abilities[abilityArg->AbilityId].AbilitySFXName, nullptr);
  sgStartCoroutine(slashCo);
  sgStartCoroutine(damageCo);
  sgStartCoroutine(co);
}

static void createAnimationGameObjects() {
  for (size_t i = 0; i < _numAbilities; i++) {
    if (_abilities[i].AnimationGameObject) {
      continue;
    }
    auto go = new GameObject();
    auto animationComp = AnimationComponent();
    auto locationComp = LocationComponent();
    auto keepAlive = KeepAliveComponent();
    animationComp.AnimationName = _abilities[i].AbilityAnimation;
    animationComp.AnimationSpeed = 1.0;
    animationComp.Visible = false;
    animationComp.Layer = 1;
    go->AddComponent<AnimationComponent>(animationComp);
    go->AddComponent<LocationComponent>(locationComp);
    go->AddComponent<KeepAliveComponent>(keepAlive);
    _abilities[i].AnimationGameObject = go;
    Events::PushEvent(Events::BuiltinEvents.GameObjectAdd, true, (void *)go);
  }
}

void Supergoon::InitializeBattleAbilitySystem() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleAbilityUsed, handleAbilityUsed);
}
void Supergoon::LoadBattleAbilities() {
  createAnimationGameObjects();
}
