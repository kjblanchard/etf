#include "Components/BattlerComponent.hpp"
#include "Supergoon/Content/ContentRegistry.hpp"
#include "Supergoon/Content/Sfx.hpp"
#include "Supergoon/ECS/Components/AnimationComponent.hpp"
#include "Supergoon/Events.hpp"
#include "Supergoon/Tween/Tween.hpp"
#include <Entities/Battle/BattleCommandArgs.hpp>
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
#include <memory>
#include <sys/_types/_u_int64_t.h>
using namespace Supergoon;
using namespace std;
static void Victory(GameState *gamestate);

static shared_ptr<Sfx> menuMoveSfx = nullptr;
static shared_ptr<Sfx> menuSelectSfx = nullptr;
static shared_ptr<Sfx> playerTurnSfx = nullptr;
static shared_ptr<Sfx> enemyDiedSfx = nullptr;
static shared_ptr<Sfx> slashSfx = nullptr;
static shared_ptr<Sfx> errorSfx = nullptr;
static bool initialized = false;
static bool battleEnded = false;
static bool battleJustStarted = true;
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
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleDamageEvent, [](int battlerId, void *damage, void *) {
    GameObject::ForEach<BattlerComponent>([battlerId, &damage](GameObject, BattlerComponent &battlerComponent) {
      if (battlerComponent.Id != battlerId) {
        return;
      }
      assert((u_int64_t)damage && "Cannot convert damage to int");
      auto damageInt = (u_int64_t)damage;
      battlerComponent.Stats.HP -= damageInt;
      // If it's a enemy, we should play a sound and then victory.
      if (battlerComponent.Stats.HP <= 0) {
        Sound::Instance()->PlaySfx(enemyDiedSfx.get());
        enemyDiedTween.Restart();
      }
      // TODO If it's a player, we should update the hp
    });
  });
  gamestate->BattleData.CurrentBattler = 1;
  menuMoveSfx = ContentRegistry::CreateContent<Sfx>("menuMove");
  menuSelectSfx = ContentRegistry::CreateContent<Sfx>("menuSelect");
  playerTurnSfx = ContentRegistry::CreateContent<Sfx>("playerTurn");
  errorSfx = ContentRegistry::CreateContent<Sfx>("error1");
  enemyDiedSfx = ContentRegistry::CreateContent<Sfx>("enemyDead");
  slashSfx = ContentRegistry::CreateContent<Sfx>("slash1");
  enemyDiedTween = Tween(1.0);
  enemyDiedTween.SetAutostart(false);
  enemyDiedTween.EndFunc = [gamestate]() {
    bool allEnemiesDead = true;
    GameObject::ForEach<BattlerComponent>([&allEnemiesDead](GameObject, BattlerComponent &battlerComp) {
      if (battlerComp.IsPlayer) {
        return;
      }
      if (battlerComp.Stats.HP > 0) {
        allEnemiesDead = false;
      }
    });
    if (allEnemiesDead) {
      Victory(gamestate);
    }
  };
  menuMoveSfx->LoadContent();
  menuSelectSfx->LoadContent();
  playerTurnSfx->LoadContent();
  enemyDiedSfx->LoadContent();
  errorSfx->LoadContent();
  slashSfx->LoadContent();
  InitializeBattleAbilitySystem();
  initialized = true;
}
static void updateATBs(GameState &gamestate) {
  if (battleEnded) {
    return;
  }
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
static void battlerStartAnimation(void *userdata) {
  auto comp = (AnimationComponent *)userdata;
  assert(comp);
  comp->Animation->PlayAnimation("slash2");
  auto co = sgAddCoroutine(
      0.25, [](void *udata) {
        // auto sfx = (Sfx *)udata;
        Sound::Instance()->PlaySfx(slashSfx.get());
      },
      slashSfx.get());
  sgStartCoroutine(co);

  auto args = new BattleCommandArgs{4, 1, 0};
  Events::PushEvent(EscapeTheFateEvents.BattleAbilityUsed, 0, (void *)args);
}
static void Victory(GameState *gamestate) {
  Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("victory"));
  GameObject::ForEach<BattlerComponent, AnimationComponent>([](GameObject, BattlerComponent &battler, AnimationComponent &anim) {
    if (!battler.IsPlayer) {
      return;
    }
    anim.Animation->PlayAnimation("cheer1");
  });
  Events::PushEvent(EscapeTheFateEvents.VictoryStart, 0);
  gamestate->BattleData.BattleVictory = true;
  battleEnded = true;
}

void handlePlayerInputForBattler(GameState *gamestate) {
  // TODO currently this happens before the UI updates (1 frame off) when a players turn starts, is this bad?
  if (battleEnded && KeyJustPressed(KeyboardKeys::Key_SPACE)) {
    Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, true, (void *)strdup((gamestate->PlayerLoadLevel.c_str())));
    Events::PushEvent(EscapeTheFateEvents.VictoryEnd, 0);
    gamestate->BattleData.BattleVictory = false;
    gamestate->InBattle = false;
    gamestate->ExitingBattle = true;
    gamestate->Loading = true;
    battleEnded = false;
    battleJustStarted = true;
    return;
  }
  if (currentBattler == -1) {
    return;
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
    if (currentFingerPos == 0) {
      Sound::Instance()->PlaySfx(menuSelectSfx.get());
      currentBattlerComp->CurrentATB = 0;
      currentBattler = -1;
      // animation and sound should happen after a wait time.
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
  enemyDiedTween.Update();
  UpdateBattleUI();
}
