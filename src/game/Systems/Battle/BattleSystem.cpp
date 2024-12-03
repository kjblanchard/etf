#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Input.hpp>
#include <Systems/Battle/BattleSystem.hpp>
using namespace Supergoon;

void Supergoon::UpdateBattle() {
	auto gamestate = GameObject::FindComponent<GameState>();
	assert(gamestate);
	if (!gamestate->InBattle) {
		return;
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		gamestate->InBattle = false;
		gamestate->ExitingBattle = true;
		gamestate->Loading = true;
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void*)strdup((gamestate->PlayerLoadLevel.c_str())));
	}
}
