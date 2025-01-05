#pragma once
#include <Supergoon/ECS/Gameobject.hpp>
namespace Supergoon {
struct BattleCommandArgs {
  Supergoon::GameObject TargetBattler;
  Supergoon::GameObject AttackingBattler;
  unsigned int AbilityId = 0;
};

} // namespace Supergoon
