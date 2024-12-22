#include <Entities/Battle/BattleState.hpp>
using namespace Supergoon;

const char *Supergoon::GetBattleStateText(BattleState state) {
  switch (state) {
  case BattleState::None:
    return "Not loaded";
  case BattleState::BattleJustStarted:
    return "BattleJustStarted";
  case BattleState::EnteringBattle:
    return "Entering Battle";
  case BattleState::Exiting:
    return "Exiting";
  case BattleState::Initialized:
    return "Initializing Battle";
  case BattleState::Victory:
    return "Victory";
  case BattleState::Battle:
    return "Battle";
  default:
    return "Default";
  }
}
