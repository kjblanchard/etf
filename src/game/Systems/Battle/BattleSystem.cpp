// #include <Supergoon/pch.hpp>
#include "Supergoon/Events.hpp"
#include "SupergoonEngine/nlohmann/json.hpp"
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Log.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Events.hpp>
using namespace Supergoon;

// are the events registered?
static bool initialized = false;
static bool battleEnded = false;
static bool battleJustStarted = true;
enum class battleStates {
  Begin,
  Initialized,
  Ready,
  Victory,
  Exit,
};
//static battleStates currentBattleState = battleStates::Begin;
// gets gamestate and checks if we are in battle.
static bool isInBattle(GameState **state) {
  auto gamestate = GameObject::FindComponent<GameState>();
  assert(gamestate);
  *state = gamestate;
  return gamestate->InBattle;
}

static void initialize() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int, void *, void *) {
    GameState *gamestate = nullptr;
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
  GameState *gamestate = nullptr;
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
  GameState *gamestate = nullptr;
  if (!isInBattle(&gamestate)) {
    return;
  }
  if (gamestate->InBattle && battleJustStarted) {
    battleJustStarted = false;
    InitializeBattleUI();
    // This should signal to the UI that we should load the UI.
    Events::PushEvent(EscapeTheFateEvents.EnterBattleFinished, 0);
  }
  // If we are loading, break here.
  if (gamestate->Loading) {
    return;
  }
  if (KeyJustPressed(KeyboardKeys::Key_Q)) {
    if (!battleEnded) {
      Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("victory"));
      Events::PushEvent(EscapeTheFateEvents.VictoryStart, 0);
      gamestate->BattleData.BattleVictory = true;
      battleEnded = true;
    } else {
      Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void *)strdup((gamestate->PlayerLoadLevel.c_str())));
      Events::PushEvent(EscapeTheFateEvents.VictoryEnd, 0);
      gamestate->BattleData.BattleVictory = false;
      gamestate->InBattle = false;
      gamestate->ExitingBattle = true;
      gamestate->Loading = true;
      battleEnded = false;
      battleJustStarted = true;
    }
  } else if (KeyJustPressed(KeyboardKeys::Key_W)) {
    Events::PushEvent(EscapeTheFateEvents.BattleButtonPressed, (int)KeyboardKeys::Key_W, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_S)) {
    Events::PushEvent(EscapeTheFateEvents.BattleButtonPressed, (int)KeyboardKeys::Key_S, nullptr);
  }

  // Switch to next turn if it is the players turn
  if (gamestate->BattleData.CurrentBattler == 1 && KeyJustPressed(KeyboardKeys::Key_N)) {
    Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, 0);
  }
  UpdateBattleUI();
}
