#include <Utilities/Events.hpp>
using namespace Supergoon;
EscapeTheFateEvent Supergoon::EscapeTheFateEvents;

void Supergoon::InitializeEvents() {
	EscapeTheFateEvents.EnterBattleFinished = Events::RegisterEvent();
	EscapeTheFateEvents.BattleTurnFinished = Events::RegisterEvent();
}
