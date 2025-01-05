#pragma once
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
  uint32_t StartEnteringBattle;
  // This is when the fadein starts so that we can load the battle system.
  uint32_t EnterBattleFinished;
  // This is used when the battle system is fully loaded and ready to load the UI system.
  uint32_t BattleFullyStarted;
  uint32_t BattleTurnFinished;
  uint32_t VictoryStart;
  uint32_t VictoryEnd;
  uint32_t commandCursorUpdate;
  uint32_t PlayerBattlerTurnBegin;
  uint32_t BattleDamageEvent;
  uint32_t BattleAbilityUsed;
  uint32_t UpdatePlayerBattlerUIEvent;
  uint32_t ShowBattleDamageTextEvent;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
} // namespace Supergoon
