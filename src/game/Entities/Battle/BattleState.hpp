#pragma once
namespace Supergoon {
enum class BattleState {
  None,
  Initialized,
  EnteringBattle,
  BattleJustStarted,
  Battle,
  Victory,
  Exiting,
};
const char *GetBattleStateText(BattleState state);
} // namespace Supergoon
