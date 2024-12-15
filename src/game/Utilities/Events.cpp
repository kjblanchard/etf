#include <Supergoon/Events.hpp>
#include <Utilities/Events.hpp>
using namespace Supergoon;
EscapeTheFateEvent Supergoon::EscapeTheFateEvents;

void Supergoon::InitializeEvents() {
  EscapeTheFateEvents.EnterBattleFinished = Events::RegisterEvent();
  EscapeTheFateEvents.BattleTurnFinished = Events::RegisterEvent();
  EscapeTheFateEvents.VictoryStart = Events::RegisterEvent();
  EscapeTheFateEvents.VictoryEnd = Events::RegisterEvent();
  EscapeTheFateEvents.BattleButtonPressed = Events::RegisterEvent();
  EscapeTheFateEvents.PlayerBattlerTurnBegin = Events::RegisterEvent();
}
