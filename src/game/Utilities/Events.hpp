#pragma once
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
  uint32_t EnterBattleFinished;
  uint32_t BattleTurnFinished;
  uint32_t VictoryStart;
  uint32_t VictoryEnd;
  uint32_t BattleButtonPressed;
  uint32_t PlayerBattlerTurnBegin;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
} // namespace Supergoon
