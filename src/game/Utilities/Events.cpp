#include <Utilities/Events.hpp>
#include <Supergoon/pch.hpp>
using namespace Supergoon;
EscapeTheFateEvent Supergoon::EscapeTheFateEvents;

void Supergoon::InitializeEvents() {
	EscapeTheFateEvents.EnterBattleFinished = Events::RegisterEvent();
	EscapeTheFateEvents.BattleTurnFinished = Events::RegisterEvent();
}
