#pragma once
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
  uint32_t EnterBattleFinished;
  uint32_t BattleTurnFinished;
  uint32_t VictoryStart;
  uint32_t VictoryEnd;
  uint32_t BattleButtonPressed;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
} // namespace Supergoon
