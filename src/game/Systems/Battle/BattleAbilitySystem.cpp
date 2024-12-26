#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleAbility.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Coroutine.h>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
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
    {0, DamageTypes::Physical, 0, 1.0f, "slash2", "slash1", "sword1"}};
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
  Events::PushEvent(EscapeTheFateEvents.BattleDamageEvent, damage, abilityArgs);
  // Start any animations of using this ability
  assert(abilityArg->AttackingBattler.HasComponent<AnimationComponent>() && "Battler doesn't have a animation component somehow");
  auto animComp = abilityArg->AttackingBattler.GetComponent<AnimationComponent>();
  animComp.Animation->PlayAnimation(ability->CharAnimationName);
  auto co = sgAddCoroutine(
      0.25, [](void *name) {
        assert((const char *)name && "Could not convert name from void* for some reason");
        Sound::Instance()->PlaySfxOneShot((const char *)name);
      },
      (void *)_abilities[abilityArg->AbilityId].AbilitySFXName);
  sgStartCoroutine(co);
}

void Supergoon::InitializeBattleAbilitySystem() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleAbilityUsed, handleAbilityUsed);
}
