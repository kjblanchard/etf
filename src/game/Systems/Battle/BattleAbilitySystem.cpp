#include <Components/BattleComponent.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Systems/Battle/BattleAbilitySystem.hpp>
#include <Utilities/Events.hpp>
#include <cassert>
using namespace Supergoon;
static GameState *gamestate = nullptr;
static BattleComponent *battleComponent = nullptr;

static void HandleAbilityUsed(int, void *abilityArgs, void *) {
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
  auto damage = 0;
  //   regular attack
  if (abilityArg->AbilityId == 0) {
    // Figure out the damage, and start the animation for it.
    damage = 1;
  }
  Events::PushEvent(EscapeTheFateEvents.BattleDamageEvent, abilityArg->TargetBattlerId, (void *)damage);
  // TODO without this it's a memory leak, just testing.
  //  delete (abilityArg);
}

void Supergoon::InitializeBattleAbilitySystem() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleAbilityUsed, HandleAbilityUsed);
}
