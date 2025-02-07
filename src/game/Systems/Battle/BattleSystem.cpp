#include "Components/BattlerComponent.hpp"
#include "Supergoon/Content/ContentRegistry.hpp"
#include "Supergoon/Content/Sfx.hpp"
#include "Supergoon/ECS/Components/AnimationComponent.hpp"
#include "Supergoon/Events.hpp"
#include "Supergoon/Tween/Tween.hpp"
#include <Components/BattleComponent.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Entities/Battle/BattleState.hpp>
#include <Supergoon/Coroutine.h>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <Systems/Battle/BattleAbilitySystem.hpp>
#include <Systems/Battle/BattleDamageSystem.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleTurnMarkerSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Systems/Battle/EnemyBattlerAISystem.hpp>
#include <Utilities/Events.hpp>
#include <cstdint>
#include <memory>
using namespace Supergoon;
using namespace std;

static shared_ptr<Sfx> menuMoveSfx = nullptr;
static shared_ptr<Sfx> menuSelectSfx = nullptr;
static shared_ptr<Sfx> playerTurnSfx = nullptr;
static shared_ptr<Sfx> errorSfx = nullptr;
static int currentBattler = -1;
static GameObject currentBattlerGameObject = GameObject(0);
static GameState *gamestate = nullptr;
static BattleComponent *battleComponent = nullptr;
// static vector<BattlerComponent *> _currentBattleBattlers;

enum class battleMenus {
  None,
  Commands,
  Magic,
  Item,
};
static battleMenus currentBattleMenu = battleMenus::None;
static const int numCommands = 3; // Attack, Magic, items
static int currentFingerPos = 0;

// Creates the battle component
static void createBattleComponent() {
  auto go = GameObject::GetGameObjectWithComponents<GameState>();
  auto battleComp = BattleComponent();
  battleComp.BattleId = 0;
  battleComp.BattleMapId = 0;
  battleComp.EnteringBattle = false;
  battleComp.InBattle = false;
  battleComp.CurrentBattleState = BattleState::None;
  go->AddComponent<BattleComponent>(battleComp);
}

static void updateATBs(GameState *gamestate, BattleComponent *battleComponent) {
  if (battleComponent->CurrentBattleState != BattleState::Battle) {
    return;
  }
  GameObject::ForEach<BattlerComponent>([gamestate](GameObject go, BattlerComponent &battleComp) {
    // TODO this could go over full atb currently, not sure if that matters
    if (battleComp.CurrentATB < battleComp.FullATB) {
      battleComp.CurrentATB += gamestate->DeltaTime * 1;
    }
    if (!battleComp.IsPlayer && battleComp.CurrentATB >= battleComp.FullATB) {
      HandleEnemyBattler(go);
      battleComp.CurrentATB = 0;
    }
  });
}

// Tries to find a ready player battler.
static int findReadyBattler() {
  auto battler = -1;
  bool foundBattler = false;
  GameObject battlerGameObject;
  // BattlerComponent *comp = nullptr;
  // AnimationComponent *animComp = nullptr;
  // GameObject::ForEach<BattlerComponent, AnimationComponent>([&foundBattler, &battler, &comp, &animComp](GameObject go, BattlerComponent &battleComponent, AnimationComponent &animComponent) {
  GameObject::ForEach<BattlerComponent, AnimationComponent>([&foundBattler, &battler, &battlerGameObject](GameObject go, BattlerComponent &battleComponent, AnimationComponent &) {
    if (foundBattler) {
      return;
    }
    if (battleComponent.IsPlayer && battleComponent.CurrentATB >= battleComponent.FullATB) {
      foundBattler = true;
      battlerGameObject = go;
      // comp = &battleComponent;
      // animComp = &animComponent;
      battler = battleComponent.Id;
    }
  });
  if (battler != -1) {
    Sound::Instance()->PlaySfx(playerTurnSfx.get());
    // TODO this is weetaded
    currentBattlerGameObject = GameObject(battlerGameObject);
    Events::PushEvent(EscapeTheFateEvents.PlayerBattlerTurnBegin, battler);
  }
  return battler;
}

static void endBattle(GameState *gamestate, BattleComponent *battleComponent) {
  Events::PushEvent(EscapeTheFateEvents.VictoryEnd, 0);
  Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void *)strdup((gamestate->PlayerLoadLevel.c_str())));
  battleComponent->InBattle = false;
  gamestate->Loading = true;
  currentBattler = -1;
  EndBattleDamageSystem();
  battleComponent->CurrentBattleState = BattleState::Exiting;
}

static void victoryUpdate(GameState *gamestate, BattleComponent *battleComponent) {
  if (KeyJustPressed(KeyboardKeys::Key_SPACE)) {
    endBattle(gamestate, battleComponent);
  }
}

void handlePlayerInputForBattler(GameState *, BattleComponent *) {
  if (currentBattler == -1) {
    return;
  }
  // If players turn, we should pop up the UI for the player and handle the input.
  else if (KeyJustPressed(KeyboardKeys::Key_W)) {
    Sound::Instance()->PlaySfx(menuMoveSfx.get());
    // currentFingerPos = --currentFingerPos < 0 ? numCommands - 1 : currentFingerPos;
    --currentFingerPos;
    if (currentFingerPos < 0) {
      currentFingerPos = numCommands - 1;
    }
    Events::PushEvent(EscapeTheFateEvents.commandCursorUpdate, currentFingerPos, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_S)) {
    currentFingerPos = ++currentFingerPos >= numCommands ? 0 : currentFingerPos;
    Sound::Instance()->PlaySfx(menuMoveSfx.get());
    Events::PushEvent(EscapeTheFateEvents.commandCursorUpdate, currentFingerPos, nullptr);
  } else if (KeyJustPressed(KeyboardKeys::Key_SPACE)) {
    // We selected something, this should end our turn, resetting the atb and hiding the menu until the player is ready again.
    if (currentFingerPos == 0) {
      Sound::Instance()->PlaySfx(menuSelectSfx.get());
      currentBattlerGameObject.GetComponent<BattlerComponent>().CurrentATB = 0;
      // Determine the attacker and target and ability.
      BattleCommandArgs *battleCommandArg = new BattleCommandArgs();
      battleCommandArg->AbilityId = 0;
      battleCommandArg->AttackingBattler = currentBattlerGameObject;
      // TODO How do we find the Target Battler Gameobject? for now, hack it to be birb.
      GameObject targetGameObject;
      GameObject::ForEach<BattlerComponent>([&targetGameObject](GameObject go, BattlerComponent battlerComp) {
        if (battlerComp.Id == 4) {
          targetGameObject = go;
        }
      });
      battleCommandArg->TargetBattler = targetGameObject;
      auto co = sgAddCoroutine(
          0.25, [](void *userdata, void *) {
            Events::PushEvent(EscapeTheFateEvents.BattleAbilityUsed, 0, (void *)userdata);
          },
          (void *)battleCommandArg, nullptr);
      sgStartCoroutine(co);
      Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, currentBattler);
      currentBattler = -1;
    } else {
      Sound::Instance()->PlaySfx(errorSfx.get());
    }
  }
}
static void initializeBattleSystem(GameState *, BattleComponent *battleComponent) {
  Events::RegisterEventHandler(EscapeTheFateEvents.EnterBattleFinished, [battleComponent](int, void *, void *) {
    if (battleComponent->CurrentBattleState == BattleState::Initialized) {
      battleComponent->CurrentBattleState = BattleState::BattleJustStarted;
    }
  });
  menuMoveSfx = ContentRegistry::CreateContent<Sfx>("menuMove");
  menuSelectSfx = ContentRegistry::CreateContent<Sfx>("menuSelect");
  playerTurnSfx = ContentRegistry::CreateContent<Sfx>("playerTurn");
  errorSfx = ContentRegistry::CreateContent<Sfx>("error1");
  menuMoveSfx->LoadContent();
  menuSelectSfx->LoadContent();
  playerTurnSfx->LoadContent();
  errorSfx->LoadContent();
  InitializeBattleUI();
  InitializeBattleAbilitySystem();
  InitializeBattleDamageSystem(battleComponent);
  InitializeBattleTurnMarkerSystem();
  battleComponent->CurrentBattleState = BattleState::BattleJustStarted;
}

static void cacheBattlers(BattleComponent *battleComponent) {
  battleComponent->Battlers.clear();
  GameObject::ForEach<BattlerComponent>([battleComponent](GameObject, BattlerComponent &battlerComp) {
    battleComponent->Battlers.push_back(&battlerComp);
  });
}

static void startBattle(GameState *, BattleComponent *battleComponent) {
  if (!battleComponent->InBattle) {
    return;
  }
  cacheBattlers(battleComponent);
  currentBattler = -1;
  currentBattleMenu = battleMenus::None;
  Events::PushEvent(EscapeTheFateEvents.BattleFullyStarted, 0);
  battleComponent->CurrentBattleState = BattleState::Battle;
}

static void exiting(GameState *, BattleComponent *battleComponent) {
  battleComponent->CurrentBattleState = BattleState::Exiting;
}

static void updateFinger() {
  if (currentBattler == -1 && (currentBattler = findReadyBattler()) != -1) {
    currentBattleMenu = battleMenus::Commands;
    currentFingerPos = 0;
  }
}
void Supergoon::InitializeBattleSystem() {
  gamestate = GameObject::FindComponent<GameState>();
  createBattleComponent();
}

void Supergoon::UpdateBattle() {
  gamestate = GameObject::FindComponent<GameState>();
  battleComponent = GameObject::FindComponent<BattleComponent>();
  if (!battleComponent->InBattle) {
    return;
  }
  switch (battleComponent->CurrentBattleState) {
  case BattleState::None:
    initializeBattleSystem(gamestate, battleComponent);
    break;
  case BattleState::Initialized:
    break;
  case BattleState::BattleJustStarted:
    startBattle(gamestate, battleComponent);
    break;
  case BattleState::Battle:
    updateATBs(gamestate, battleComponent);
    updateFinger();
    handlePlayerInputForBattler(gamestate, battleComponent);
    UpdateBattleDamageSystem();
    UpdateBattleUI();
    break;
  case BattleState::Exiting:
    exiting(gamestate, battleComponent);
    break;
  case BattleState::Victory:
    victoryUpdate(gamestate, battleComponent);
  default:
    break;
  }
}
