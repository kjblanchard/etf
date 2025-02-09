#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Supergoon/Events.hpp>
#include <Systems/Battle/EnemyBattlerAISystem.hpp>
#include <Utilities/Events.hpp>
using namespace Supergoon;
using namespace std;

static GameObject determineTarget() {
  auto returnGO = GameObject(0);
  // TODO this should be done differently.
  GameObject::ForEach<BattlerComponent>([&returnGO](GameObject go, BattlerComponent &battler) {
    if (battler.Id == 1) {
      returnGO = go;
    }
  });
  return returnGO;
}

void Supergoon::HandleEnemyBattler(GameObject battler) {
  assert(battler.HasComponent<BattlerComponent>() && "Does not have proper components on enemy");
  auto &battlerComp = battler.GetComponent<BattlerComponent>();
  if (IsBattlerDead(battlerComp)) {
    return;
  }
  auto args = new BattleCommandArgs();
  args->AbilityId = 0;
  args->AttackingBattler = battler;
  args->TargetBattler = determineTarget();
  Events::PushEvent(EscapeTheFateEvents.BattleAbilityUsed, 0, (void *)args);
}
