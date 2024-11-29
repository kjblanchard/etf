#pragma once
#include <Supergoon/Events.hpp>
#include <cstdint>
namespace Supergoon {
struct EscapeTheFateEvent {
	uint32_t EnterBattleFinished;
};
extern EscapeTheFateEvent EscapeTheFateEvents;
void InitializeEvents();
}  // namespace Supergoon
