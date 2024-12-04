#pragma once
#include <Supergoon/Events.hpp>
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
	uint32_t EnterBattleFinished;
	uint32_t BattleTurnFinished;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
}  // namespace Supergoon
