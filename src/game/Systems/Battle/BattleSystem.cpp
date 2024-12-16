// #include <Supergoon/pch.hpp>
#include "Components/BattlerComponent.hpp"
#include "Supergoon/Content/ContentRegistry.hpp"
#include "Supergoon/Content/Sfx.hpp"
#include "Supergoon/ECS/Components/AnimationComponent.hpp"
#include "Supergoon/Events.hpp"
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

enum class battleMenus {
  None,
  Commands,
  Magic,
  Item,
};
static battleMenus currentBattleMenu = battleMenus::None;
static const int numCommands = 3; // Attack, Magic, items
static int currentFingerPos = 0;

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
  auto battler = -1;
  bool foundBattler = false;
  BattlerComponent *comp = nullptr;
  AnimationComponent *animComp = nullptr;
  GameObject::ForEach<BattlerComponent, AnimationComponent>([&foundBattler, &battler, &comp, &animComp](GameObject, BattlerComponent &battleComponent, AnimationComponent &animComponent) {
    if (foundBattler) {
      return;
    }
    if (battleComponent.IsPlayer && battleComponent.CurrentATB >= battleComponent.FullATB) {
      foundBattler = true;
      comp = &battleComponent;
      animComp = &animComponent;
      battler = battleComponent.Id;
    }
  });
  if (battler != -1) {
    Sound::Instance()->PlaySfx(playerTurnSfx.get());
    currentBattlerComp = comp;
    currentBattlerAnimationComp = animComp;
    Events::PushEvent(EscapeTheFateEvents.PlayerBattlerTurnBegin, battler);
  }
  return battler;
}

void handlePlayerInputForBattler(GameState *gamestate) {
  if (currentBattler == -1) {
    return;
  }
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
    currentFingerPos = --currentFingerPos < 0 ? numCommands - 1 : currentFingerPos;
    Events::PushEvent(EscapeTheFateEvents.commandCursorUpdate, currentFingerPos, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_S)) {
    currentFingerPos = ++currentFingerPos >= numCommands ? 0 : currentFingerPos;
    Sound::Instance()->PlaySfx(menuMoveSfx.get());
    Events::PushEvent(EscapeTheFateEvents.commandCursorUpdate, currentFingerPos, nullptr);
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
    currentBattler = -1;
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
    currentBattleMenu = battleMenus::None;
    InitializeBattleUI();
    // This should signal to the UI that we should load the UI.
    Events::PushEvent(EscapeTheFateEvents.EnterBattleFinished, 0);
  }
  // If we are loading, break here.
  if (gamestate->Loading) {
    return;
  }
  updateATBs(*gamestate);
  if (currentBattler == -1 && (currentBattler = findReadyBattler()) != -1) {
    currentBattleMenu = battleMenus::Commands;
    currentFingerPos = 0;
  }
  handlePlayerInputForBattler(gamestate);
  UpdateBattleUI();
}
