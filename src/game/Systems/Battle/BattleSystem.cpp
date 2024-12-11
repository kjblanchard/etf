// #include <Supergoon/pch.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Log.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Utilities/Events.hpp>
using namespace Supergoon;

// are the events registered?
static bool initialized = false;
static bool battleEnded = false;
// gets gamestate and checks if we are in battle.
static bool isInBattle(GameState** state) {
	auto gamestate = GameObject::FindComponent<GameState>();
	assert(gamestate);
	*state = gamestate;
	return gamestate->InBattle;
}

static void initialize() {
	Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int, void*, void*) {
		GameState* gamestate = nullptr;
		if (!isInBattle(&gamestate)) {
			return;
		}
		// TODO, currently alternate between the enemy and the player while testing.
		sgLogWarn("Current turn is ending for %d, and next turn is %d", gamestate->BattleData.CurrentBattler, 4);
		if (gamestate->BattleData.CurrentBattler == 1) {
			gamestate->BattleData.CurrentBattler = 4;
		} else {
			gamestate->BattleData.CurrentBattler = 1;
		}
	});
	GameState* gamestate = nullptr;
	if (!isInBattle(&gamestate)) {
		return;
	}
	// determine whose turn it is first, currently set it to the player.
	gamestate->BattleData.CurrentBattler = 1;
	initialized = true;
}

void Supergoon::UpdateBattle() {
	// TODO do this differently to save calls every frame.
	if (!initialized) {
		initialize();
	}
	GameState* gamestate = nullptr;
	if (!isInBattle(&gamestate)) {
		return;
	}
	if (!gamestate->Loading && KeyJustPressed(KeyboardKeys::Key_W)) {
		if (!battleEnded) {
			Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void*)strdup("victory"));
			gamestate->BattleData.BattleVictory = true;
			battleEnded = true;
		} else {
			Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void*)strdup((gamestate->PlayerLoadLevel.c_str())));
			gamestate->BattleData.BattleVictory = false;
			gamestate->InBattle = false;
			gamestate->ExitingBattle = true;
			gamestate->Loading = true;
			battleEnded = false;
		}
	}
	// Switch to next turn if it is the players turn
	if (gamestate->BattleData.CurrentBattler == 1 && KeyJustPressed(KeyboardKeys::Key_N)) {
		Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, 0);
	}
}
