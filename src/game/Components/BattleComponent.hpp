#pragma once
#include <Entities/Battle/BattleState.hpp>
namespace Supergoon {
struct BattleComponent {
  // Battle group ID of the next battle
  int BattleId;
  int BattleMapId;
  bool EnteringBattle = false;
  bool InBattle = false;
  BattleState CurrentBattleState = BattleState::None;
};
} // namespace Supergoon
