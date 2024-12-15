// #include <Supergoon/pch.hpp>
#include "Components/BattlerComponent.hpp"
#include "Supergoon/Content/ContentRegistry.hpp"
#include "Supergoon/Content/Sfx.hpp"
#include "Supergoon/ECS/Components/AnimationComponent.hpp"
#include "Supergoon/Events.hpp"
#include "SupergoonEngine/log.h"
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Events.hpp>
#include <memory>
using namespace Supergoon;
using namespace std;

// are the events registered?
static shared_ptr<Sfx> menuMoveSfx = nullptr;
static shared_ptr<Sfx> menuSelectSfx = nullptr;
static shared_ptr<Sfx> playerTurnSfx = nullptr;
static bool initialized = false;
static bool battleEnded = false;
static bool battleJustStarted = true;
static int currentBattler = -1;
static BattlerComponent *currentBattlerComp = nullptr;
static AnimationComponent *currentBattlerAnimationComp = nullptr;
enum class battleStates {
  Begin,
  Initialized,
  Ready,
  Victory,
  Exit,
};
// static battleStates currentBattleState = battleStates::Begin;
//  gets gamestate and checks if we are in battle.
static bool isInBattle(GameState **state) {
  auto gamestate = GameObject::FindComponent<GameState>();
  assert(gamestate);
  *state = gamestate;
  return gamestate->InBattle;
}

static void initialize() {
  GameState *gamestate = nullptr;
  if (!isInBattle(&gamestate)) {
    return;
  }
  // determine whose turn it is first, currently set it to the player.
  gamestate->BattleData.CurrentBattler = 1;
  menuMoveSfx = ContentRegistry::CreateContent<Sfx>("menuMove");
  menuSelectSfx = ContentRegistry::CreateContent<Sfx>("menuSelect");
  playerTurnSfx = ContentRegistry::CreateContent<Sfx>("playerTurn");
  menuMoveSfx->LoadContent();
  menuSelectSfx->LoadContent();
  playerTurnSfx->LoadContent();
  initialized = true;
}
static void updateATBs(GameState &gamestate) {
  GameObject::ForEach<BattlerComponent>([&gamestate](GameObject, BattlerComponent &battleComp) {
    battleComp.CurrentATB = battleComp.CurrentATB < battleComp.FullATB ? battleComp.CurrentATB += gamestate.DeltaTime * 1 : battleComp.FullATB;
  });
}

static int findReadyBattler() {
  // sgLogWarn("Need to find a ready battler...");
  auto battler = -1;
  bool foundBattler = false;
  BattlerComponent *comp = nullptr;
  AnimationComponent *animComp = nullptr;
  GameObject::ForEach<BattlerComponent, AnimationComponent>([&foundBattler, &battler, &comp, &animComp](GameObject, BattlerComponent &battleComponent, AnimationComponent &animComponent) {
    if (foundBattler) {
      return;
    }
    // sgLogWarn("Checking battler %d, who has atb of %f of %f", battleComponent.Id, battleComponent.CurrentATB, battleComponent.FullATB);
    if (battleComponent.IsPlayer && battleComponent.CurrentATB >= battleComponent.FullATB) {
      foundBattler = true;
      comp = &battleComponent;
      animComp = &animComponent;
      battler = battleComponent.Id;
    }
  });
  if (battler != -1) {
    // sgLogWarn("Battler found, pushing event");
    Sound::Instance()->PlaySfx(playerTurnSfx.get());
    currentBattlerComp = comp;
    currentBattlerAnimationComp = animComp;
    Events::PushEvent(EscapeTheFateEvents.PlayerBattlerTurnBegin, battler);
  }
  return battler;
}

void handlePlayerInputForBattler(GameState *gamestate) {
  // TODO currently this happens before the UI updates (1 frame off) when a players turn starts, is this bad?
  if (KeyJustPressed(KeyboardKeys::Key_Q)) {
    if (!battleEnded) {
      Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("victory"));
      Events::PushEvent(EscapeTheFateEvents.VictoryStart, 0);
      currentBattlerAnimationComp->Animation->PlayAnimation("cheer1");
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
  }
  // If players turn, we should pop up the UI for the player and handle the input.
  else if (KeyJustPressed(KeyboardKeys::Key_W)) {
    Sound::Instance()->PlaySfx(menuMoveSfx.get());
    Events::PushEvent(EscapeTheFateEvents.BattleButtonPressed, (int)KeyboardKeys::Key_W, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_S)) {
    Sound::Instance()->PlaySfx(menuMoveSfx.get());
    Events::PushEvent(EscapeTheFateEvents.BattleButtonPressed, (int)KeyboardKeys::Key_S, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_SPACE)) {
    // We selected something, this should end our turn, resetting the atb and hiding the menu until the player is ready again.
    Sound::Instance()->PlaySfx(menuSelectSfx.get());
    currentBattlerComp->CurrentATB = 0;
    currentBattler = -1;
    currentBattlerAnimationComp->Animation->PlayAnimation("slash2");
    Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, 0);
    currentBattlerAnimationComp = nullptr;
    currentBattlerComp = nullptr;
  }
  // Switch to next turn if it is the players turn
  if (gamestate->BattleData.CurrentBattler == 1 && KeyJustPressed(KeyboardKeys::Key_N)) {
    Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, 0);
  }
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
    currentBattler = -1;
    currentBattlerComp = nullptr;
    battleJustStarted = false;
    InitializeBattleUI();
    // This should signal to the UI that we should load the UI.
    Events::PushEvent(EscapeTheFateEvents.EnterBattleFinished, 0);
  }
  // If we are loading, break here.
  if (gamestate->Loading) {
    return;
  }
  updateATBs(*gamestate);
  // Handle exiting the battle for testing.
  currentBattler = currentBattler == -1 ? findReadyBattler() : currentBattler;
  if (currentBattler >= 0) {
    handlePlayerInputForBattler(gamestate);
  }
  UpdateBattleUI();
}
