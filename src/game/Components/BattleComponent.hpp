#pragma once
#include <Entities/Battle/BattleState.hpp>
#include <vector>
namespace Supergoon {
struct BattlerComponent;
struct BattleComponent {
  // Battle group ID of the next battle
  int BattleId = 0;
  int BattleMapId = 0;
  bool EnteringBattle = false;
  bool InBattle = false;
  std::vector<BattlerComponent *> Battlers;
  BattleState CurrentBattleState = BattleState::None;
};
} // namespace Supergoon
