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
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Events.hpp>
#include <cstdint>
#include <memory>
using namespace Supergoon;
using namespace std;
static void startVictory(GameState *gamestate, BattleComponent *battleComponent);

static shared_ptr<Sfx> menuMoveSfx = nullptr;
static shared_ptr<Sfx> menuSelectSfx = nullptr;
static shared_ptr<Sfx> playerTurnSfx = nullptr;
static shared_ptr<Sfx> enemyDiedSfx = nullptr;
static shared_ptr<Sfx> slashSfx = nullptr;
static shared_ptr<Sfx> errorSfx = nullptr;
// static bool battleEnded = false;
// static bool battleJustStarted = true;
static int currentBattler = -1;
static Tween enemyDiedTween = Tween(1.0);
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
static bool isInBattle(GameState **state, BattleComponent **battleState) {
  if (!*state) {
    auto gamestate = GameObject::FindComponent<GameState>();
    if (!gamestate) {
      return false;
    }
    *state = gamestate;
  }
  if (!*battleState) {
    auto battlestate = GameObject::FindComponent<BattleComponent>();
    *battleState = battlestate;
  }
  return (*battleState)->InBattle;
}

static void updateATBs(GameState &gamestate, BattleComponent *battleComponent) {
  if (battleComponent->CurrentBattleState != BattleState::Battle) {
    return;
  }
  GameObject::ForEach<BattlerComponent>([&gamestate](GameObject, BattlerComponent &battleComp) {
    // TODO this could go over full atb currently, not sure if that matters
    if (battleComp.CurrentATB < battleComp.FullATB) {
      battleComp.CurrentATB += gamestate.DeltaTime * 1;
    }
    // battleComp.CurrentATB = battleComp.CurrentATB < battleComp.FullATB ? battleComp.CurrentATB += gamestate.DeltaTime * 1 : battleComp.FullATB;
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
static void battlerStartAnimation(void *userdata) {
  auto comp = (AnimationComponent *)userdata;
  assert(comp);
  comp->Animation->PlayAnimation("slash2");
  // auto co = sgAddCoroutine(
  //     0.25, [](void *) {
  //       // auto sfx = (Sfx *)udata;
  //       Sound::Instance()->PlaySfx(slashSfx.get());
  //     },
  //     slashSfx.get());
  // sgStartCoroutine(co);

  auto args = new BattleCommandArgs{4, 1, 0};
  Events::PushEvent(EscapeTheFateEvents.BattleAbilityUsed, 0, (void *)args);
}
static void startVictory(GameState *gamestate, BattleComponent *battleComponent) {
  Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("victory"));
  GameObject::ForEach<BattlerComponent, AnimationComponent>([](GameObject, BattlerComponent &battler, AnimationComponent &anim) {
    if (!battler.IsPlayer) {
      return;
    }
    anim.Animation->PlayAnimation("cheer1");
  });
  Events::PushEvent(EscapeTheFateEvents.VictoryStart, 0);
  battleComponent->CurrentBattleState = BattleState::Victory;
}

static void EndBattle(GameState *gamestate, BattleComponent *battleComponent) {
  Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void *)strdup((gamestate->PlayerLoadLevel.c_str())));
  Events::PushEvent(EscapeTheFateEvents.VictoryEnd, 0);
  battleComponent->InBattle = false;
  gamestate->Loading = true;
  // battleJustStarted = true;
  currentBattler = -1;
  battleComponent->CurrentBattleState = BattleState::Exiting;
}

static void victoryUpdate(GameState *gamestate, BattleComponent *battleComponent) {
  if (KeyJustPressed(KeyboardKeys::Key_SPACE)) {
    EndBattle(gamestate, battleComponent);
  }
}

void handlePlayerInputForBattler(GameState *gamestate, BattleComponent *) {
  // TODO currently this happens before the UI updates (1 frame off) when a players turn starts, is this bad?
  // If there is no battler ready, return
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
      currentBattlerComp->CurrentATB = 0;
      currentBattler = -1;
      // animation and sound should happen after a wait time.
      // auto co = sgAddCoroutine(0.25, battlerStartAnimation, currentBattlerAnimationComp);
      battlerStartAnimation(currentBattlerAnimationComp);
      auto co = sgAddCoroutine(0.25, battlerStartAnimation, currentBattlerAnimationComp);
      sgStartCoroutine(co);
      //
      Events::PushEvent(EscapeTheFateEvents.BattleTurnFinished, 0);
      currentBattlerAnimationComp = nullptr;
      currentBattlerComp = nullptr;
    } else {
      Sound::Instance()->PlaySfx(errorSfx.get());
    }
  }
}
static void initializeBattleSystem(GameState *gamestate, BattleComponent *battleComponent) {
  Events::RegisterEventHandler(EscapeTheFateEvents.EnterBattleFinished, [battleComponent](int, void *, void *) {
    if (battleComponent->CurrentBattleState == BattleState::Initialized) {
      battleComponent->CurrentBattleState = BattleState::BattleJustStarted;
    }
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleDamageEvent, [](int battlerId, void *damage, void *) {
    GameObject::ForEach<BattlerComponent>([battlerId, &damage](GameObject, BattlerComponent &battlerComponent) {
      if (battlerComponent.Id != battlerId) {
        return;
      }
      // assert((u_int64_t)damage && "Cannot convert damage to int");
      // auto damageInt = (u_int64_t)damage;
      auto damageInt = 1;
      battlerComponent.Stat.HP -= damageInt;
      // If it's a enemy, we should play a sound and then victory.
      if (battlerComponent.Stat.HP <= 0) {
        Sound::Instance()->PlaySfx(enemyDiedSfx.get());
        enemyDiedTween.Restart();
      }
      // TODO If it's a player, we should update the hp
    });
  });
  menuMoveSfx = ContentRegistry::CreateContent<Sfx>("menuMove");
  menuSelectSfx = ContentRegistry::CreateContent<Sfx>("menuSelect");
  playerTurnSfx = ContentRegistry::CreateContent<Sfx>("playerTurn");
  errorSfx = ContentRegistry::CreateContent<Sfx>("error1");
  enemyDiedSfx = ContentRegistry::CreateContent<Sfx>("enemyDead");
  slashSfx = ContentRegistry::CreateContent<Sfx>("slash1");
  enemyDiedTween = Tween(1.0);
  enemyDiedTween.SetAutostart(false);
  enemyDiedTween.EndFunc = [gamestate, battleComponent]() {
    if (battleComponent->CurrentBattleState == BattleState::Victory) {
      return;
    }
    bool allEnemiesDead = true;
    sgLogWarn("Running endfunc");
    GameObject::ForEach<BattlerComponent>([&allEnemiesDead](GameObject, BattlerComponent &battlerComp) {
      if (battlerComp.IsPlayer) {
        return;
      }
      if (battlerComp.Stat.HP > 0) {
        allEnemiesDead = false;
      }
    });
    if (allEnemiesDead) {
      startVictory(gamestate, battleComponent);
    }
  };
  menuMoveSfx->LoadContent();
  menuSelectSfx->LoadContent();
  playerTurnSfx->LoadContent();
  enemyDiedSfx->LoadContent();
  errorSfx->LoadContent();
  slashSfx->LoadContent();
  // This runs the initial initialization of the UI, that registers the event handlers.
  InitializeBattleUI();
  InitializeBattleAbilitySystem();
  battleComponent->CurrentBattleState = BattleState::BattleJustStarted;
}

static void startBattle(GameState *, BattleComponent *battleComponent) {
  if (battleComponent->InBattle) {
    currentBattler = -1;
    currentBattlerComp = nullptr;
    currentBattleMenu = battleMenus::None;
    // Load the battle ui next frame.
    Events::PushEvent(EscapeTheFateEvents.BattleFullyStarted, 0);
    battleComponent->CurrentBattleState = BattleState::Battle;
  }
}
static void exiting(GameState *, BattleComponent *battleComponent) {
  battleComponent->CurrentBattleState = BattleState::Exiting;
}

void Supergoon::UpdateBattle() {
  GameState *gamestate = nullptr;
  BattleComponent *battleComponent = nullptr;
  if (!isInBattle(&gamestate, &battleComponent)) {
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
    updateATBs(*gamestate, battleComponent);
    if (currentBattler == -1 && (currentBattler = findReadyBattler()) != -1) {
      currentBattleMenu = battleMenus::Commands;
      currentFingerPos = 0;
    }
    handlePlayerInputForBattler(gamestate, battleComponent);
    enemyDiedTween.Update();
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
