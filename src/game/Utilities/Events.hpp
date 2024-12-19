#pragma once
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
  uint32_t EnterBattleFinished;
  uint32_t BattleTurnFinished;
  uint32_t VictoryStart;
  uint32_t VictoryEnd;
  uint32_t commandCursorUpdate;
  uint32_t PlayerBattlerTurnBegin;
  uint32_t BattleDamageEvent;
  uint32_t BattleAbilityUsed;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
} // namespace Supergoon
