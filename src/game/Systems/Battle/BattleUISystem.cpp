#include "SDL3/SDL_filesystem.h"
#include "Supergoon/Primitives/Point.h"
#include "Supergoon/Primitives/Rectangle.hpp"
#include "Supergoon/UI/UIImage.hpp"
#include "SupergoonEngine/log.h"
#include "Utilities/Events.hpp"
#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Entities/Battle/BattleState.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIAnimation.hpp>
#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
#include <Supergoon/UI/UIProgressBar.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
#include <Utilities/Utilities.hpp>
#include <deque>
#include <memory>
#include <queue>
#include <sstream>
#pragma clang diagnostic ignored "-Wsign-compare"
using namespace Supergoon;
using namespace std;
static const int numDamageTextsToLoad = 10;
static const int playerBattlerSize = 3;
static const int battleCommandsSize = 3;
static const char *battleCommands[] = {"Attack", "Magic", "Item"};

static Panel *battlePanel = nullptr;
static Panel *battleCommandPanel = nullptr;
static UIImage *battleFinger = nullptr;
static UIText *battleCommandTexts[battleCommandsSize];
static Sequence fingerSequence;
static bool fingerPosChanged = false;
struct PlayerUIPanel {
  int BattlerID;
  UIHorizontalLayoutGroup *LayoutGroup;
  UIText *Name;
  UIText *HP;
  UIText *MP;
  // UIText *Speed;
  UIProgressBar *ATBBar;
};
static PlayerUIPanel _playerUIPanels[3];
static deque<UIText *> _damageTexts;

static void createDamageText(UIObject *parent) {
  for (size_t i = 0; i < numDamageTextsToLoad; i++) {
    auto text = new UIText(parent, "0", std::string("damageText") + to_string(i), 12);
    // auto textAnimator = make_shared<UIObjectAnimatorBase>(255, 0, 0.5, text->AlphaHandle());
    auto textAnimator = make_shared<UIObjectAnimatorBase>("texttweener");
    auto tweener = new Tween(255, 0, 0.75, text->AlphaHandle(), Supergoon::Easings::Linear, 0);
    textAnimator->AddUIObjectTween(tweener, text);
    text->Animators.push_back(textAnimator);
    tweener->EndFunc = [text]() {
      text->SetVisible(false);
      _damageTexts.push_back(text);
      // _damageTexts.push(text);
    };
    _damageTexts.push_back(text);
    // _damageTexts.push(text);
  }
}
static void clearDamageTextTweens() {
  for (auto &&damageText : _damageTexts) {
    damageText->Animators.erase(damageText->Animators.begin() + 1, damageText->Animators.end());
  }
}

static void createPlayersPanel(UIObject *parent) {
  auto uiTextSize = 12;
  for (size_t i = 0; i < 3; i++) {
    _playerUIPanels[i].LayoutGroup = new UIHorizontalLayoutGroup(parent, to_string(i) + "battlerhoriGroup");
    _playerUIPanels[i].LayoutGroup->XSpaceBetweenElements = 140;
    _playerUIPanels[i].Name = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "name", uiTextSize);
    _playerUIPanels[i].HP = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "hp", uiTextSize);
    _playerUIPanels[i].HP->Offset.X = 27;
    _playerUIPanels[i].MP = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "mp", uiTextSize);
    _playerUIPanels[i].MP->Offset.X = -13;
    auto path = std::string(SDL_GetBasePath()) + "assets/img/atbBar.png";
    auto atbBar = ContentRegistry::CreateContent<Image>(path);
    atbBar->LoadContent();
    _playerUIPanels[i].ATBBar = new UIProgressBar(_playerUIPanels[i].LayoutGroup, to_string(i) + "atb", "atbBar");
    _playerUIPanels[i].ATBBar->SetLayer(3);
    _playerUIPanels[i].ATBBar->BarOffset = {9, 13};
    _playerUIPanels[i].ATBBar->BarSize = {46, 4};
    _playerUIPanels[i].ATBBar->ProgressBarColor = {0, 140, 0, 255};
    _playerUIPanels[i].ATBBar->Offset.X = -70;
    _playerUIPanels[i].ATBBar->Offset.Y = -7;
    _playerUIPanels[i].ATBBar->ProgressBarAnimation->OverrideDrawSize.X = 64;
    _playerUIPanels[i].ATBBar->ProgressBarAnimation->OverrideDrawSize.Y = 32;
    _playerUIPanels[i].ATBBar->SetLayer(2);
    _playerUIPanels[i].ATBBar->SetVisible(false);
  }
}
static void assignBattlersToPlayerPanels(int slot, int ID) {
  _playerUIPanels[slot].BattlerID = ID;
  _playerUIPanels[slot].ATBBar->SetVisible(true);
}

static void updateSpeed() {
  GameObject::ForEach<BattlerComponent>([](GameObject, BattlerComponent &battler) {
    for (size_t i = 0; i < playerBattlerSize; i++) {
      if (_playerUIPanels[i].BattlerID != battler.Id) {
        continue;
      }
      _playerUIPanels[i].ATBBar->SetBarPercent(battler.CurrentATB / battler.FullATB * 100);
      // _playerUIPanels[i].Speed->UpdateText(to_string((int)(battler.CurrentATB / battler.FullATB * 100)));
    }
  });
}

static void updatePlayersPanel(BattlerComponent *comp) {
  for (size_t i = 0; i < playerBattlerSize; i++) {
    if (_playerUIPanels[i].BattlerID != comp->Id) {
      continue;
    }
    ostringstream stringBuilder;
    stringBuilder << comp->Stat.Name;
    _playerUIPanels[i].Name->UpdateText(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder << "HP: " << comp->Stat.HP << " | " << comp->Stat.MaxHP;
    _playerUIPanels[i].HP->UpdateText(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder << "MP: " << 0 << " | " << 0;
    _playerUIPanels[i].MP->UpdateText(stringBuilder.str());
    stringBuilder.str("");
    stringBuilder << comp->CurrentATB / comp->FullATB;
  }
}

static void initializePlayerUI() {
  auto fullSize = sgPoint{100, 65};
  battleCommandPanel = CreateUIBasePanel("battleCommandPanel", {110, 215}, fullSize, 255);
  battleCommandPanel->SetLayer(1);
  auto verticalLayoutGroup = new UIVerticalLayoutGroup(battleCommandPanel, "battleCommandVLG");
  verticalLayoutGroup->SetLayer(2);
  verticalLayoutGroup->YSpaceBetweenElements = 17;
  verticalLayoutGroup->Offset = {18, 9};
  for (int i = 0; i < battleCommandsSize; ++i) {
    battleCommandTexts[i] = new UIText(verticalLayoutGroup, battleCommands[i], battleCommands[i], 12);
  }
  // Currently gray these out, as they can't be used
  battleCommandTexts[1]->SetColor({160, 160, 160, 255});
  battleCommandTexts[2]->SetColor({160, 160, 160, 255});
}

static void initializeFinger() {
  auto path = std::string(SDL_GetBasePath()) + "assets/img/fingers.png";
  auto fingerImage = ContentRegistry::CreateContent<Image>(path);
  fingerImage->LoadContent();
  battleFinger = new UIImage(battleCommandPanel, "battleFinger");
  battleFinger->ImagePtr = fingerImage;
  battleFinger->Bounds.X = battleFinger->Bounds.W = 16;
  battleFinger->Bounds.H = 16;
  battleFinger->ImageSourceRect = {16, 48, 16, 16};
  battleFinger->SetLayer(3);
  battleCommandPanel->SetVisible(false);
  UI::UIInstance->UpdateInternal();
  fingerPosChanged = true;
}

static void initializeBattleUI() {
  // TODO this should happen on start, so that we can load the battlers ids properly.
  auto fullSize = sgPoint{435, 66};
  battlePanel = CreateUIBasePanel("battleBasePanel", {39, 222}, fullSize, 255);
  auto verticalLayoutGroup = new UIVerticalLayoutGroup(battlePanel, "battlersVerticalGroup");
  verticalLayoutGroup->SetLayer(1);
  verticalLayoutGroup->YSpaceBetweenElements = 16;
  verticalLayoutGroup->Offset = {9, 9};
  createPlayersPanel(verticalLayoutGroup);
  initializePlayerUI();
  initializeFinger();
  createDamageText(battlePanel);
}

static void handleInput(int loc) {
  auto battleComponent = GameObject::FindComponent<BattleComponent>();
  if (!battlePanel || !battleFinger || !battleComponent || battleComponent->CurrentBattleState != BattleState::Battle) {
    return;
  }
  auto currentFingerPos = loc;
  auto text = battleCommandTexts[currentFingerPos];
  if (!text) {
    return;
  }
  auto x = text->TextDrawRect.X - 5 - battleFinger->ImageSourceRect.W;
  auto y = text->TextDrawRect.Y;
  battleFinger->SetDrawOverride({*battleFinger->DrawOverrideXHandle(), y});
  text->SetDirty();
  // UI::UIInstance->UpdateInternal();
  float *xHandle = &battleFinger->DrawOverrideHandle()->X;
  fingerSequence = Sequence();
  auto fingerbackTween = new Tween(x, x - 5, 1.0, xHandle, Easings::Linear, -1);
  auto fingerForwardTween = new Tween(x - 5, x, 1.0, xHandle, Easings::Linear, -1);
  fingerForwardTween->UpdateFunc = []() {
    if (!battleFinger) {
      return;
    }
    battleFinger->SetDirty();
  };
  fingerbackTween->UpdateFunc = []() {
    if (!battleFinger) {
      return;
    }
    battleFinger->SetDirty();
  };
  fingerSequence.Tweens.push_back(shared_ptr<Tween>(fingerForwardTween));
  fingerSequence.Tweens.push_back(shared_ptr<Tween>(fingerbackTween));
}

static void battleUpdate() {
  if (!battlePanel || !battleCommandPanel) {
    return;
  }
  fingerSequence.Update();
  if (fingerSequence.IsComplete()) {
    fingerSequence.Restart();
  };
  updateSpeed();
}

static void battleVictory() {
  battlePanel->SetVisible(false);
  battleCommandPanel->SetVisible(false);
}

static void battleCleanup() {
  fingerPosChanged = false;
  clearDamageTextTweens();
}

static void startBattleUI() {
  static int num = 0;
  // Set all ui battlers to 0 to start, then assign them all.
  for (size_t i = 0; i < playerBattlerSize; i++) {
    _playerUIPanels[i].BattlerID = 0;
  }

  GameObject::ForEach<BattlerComponent>([](GameObject, BattlerComponent &battlerComp) {
    if (!battlerComp.IsPlayer) {
      return;
    }
    assignBattlersToPlayerPanels(num, battlerComp.Id);
    updatePlayersPanel(&battlerComp);
    ++num;
  });
  num = 0;
  battlePanel->SetVisible(true);
}
static void startBattlerATBAnim(int id) {
  for (size_t i = 0; i < playerBattlerSize; i++) {
    if (_playerUIPanels[i].BattlerID == id) {
      _playerUIPanels[i].ATBBar->ProgressBarAnimation->Animation->PlayAnimation("turn");
      _playerUIPanels[i].ATBBar->ProgressBarAnimation->Playing = true;
    }
  }
}

static void endBattlerATBAnim(int id) {
  for (size_t i = 0; i < playerBattlerSize; i++) {
    if (_playerUIPanels[i].BattlerID == id) {
      _playerUIPanels[i].ATBBar->ProgressBarAnimation->Animation->PlayAnimation("idle");
    }
  }
}

void Supergoon::InitializeBattleUI() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleFullyStarted, [](int, void *, void *) {
    startBattleUI();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int battler, void *, void *) {
    battleCommandPanel->SetVisible(false);
    fingerPosChanged = true;
    endBattlerATBAnim(battler);
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.UpdatePlayerBattlerUIEvent, [](int, void *battlerComp, void *) {
    assert((BattlerComponent *)battlerComp);
    auto comp = (BattlerComponent *)battlerComp;
    updatePlayersPanel(comp);
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.PlayerBattlerTurnBegin, [](int battler, void *, void *) {
    if (battleCommandPanel && battleFinger) {
      battleCommandPanel->SetVisible(true);
      handleInput(0);
      battleCommandPanel->SetDirty();
      startBattlerATBAnim(battler);
    }
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.commandCursorUpdate, [](int buttonLoc, void *, void *) {
    if (!battlePanel || !battleFinger || !battleCommandPanel) {
      return;
    }
    handleInput(buttonLoc);
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.VictoryStart, [](int, void *, void *) {
    battleVictory();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.VictoryEnd, [](int, void *, void *) {
    battleCleanup();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.ShowBattleDamageTextEvent, [](int damage, void *abilityArgsVoid, void *) {
    assert((BattleCommandArgs *)abilityArgsVoid && "Could not convert properly~!");
    auto abilityArgs = (BattleCommandArgs *)abilityArgsVoid;
    auto location = abilityArgs->TargetBattler.GetComponent<LocationComponent>().Location;
    if (_damageTexts.empty()) {
      return;
    }
    auto text = _damageTexts.front();
    text->UpdateText(to_string(damage));
    text->SetDrawOverride({location.X - 14, location.Y});
    auto numJump = 4;
    auto textStart = location.Y - 5;
    auto textAnimator = make_shared<UIObjectAnimatorBase>("texttweenery");
    auto tweener = new Tween(textStart, textStart - numJump, 0.2, text->DrawOverrideYHandle(), Supergoon::Easings::Linear, 0);
    auto tweener2 = new Tween(textStart - numJump, textStart, 0.5, text->DrawOverrideYHandle(), Supergoon::Easings::Linear, 0);
    textAnimator->AddUIObjectTween(tweener, text);
    textAnimator->AddUIObjectTween(tweener2, text);
    text->Animators.push_back(textAnimator);
    auto numAnim = text->Animators.size() - 1;
    // animator[0] is the fade, so it never changes.. numanim is the movement of the text.
    text->Animators[0]->Play();
    text->Animators[0]->Restart();
    text->Animators[numAnim]->Play();
    text->Animators[numAnim]->Restart();
    text->SetVisible(true);
    _damageTexts.pop_front();
    delete (abilityArgs);
  });
  initializeBattleUI();
}

void Supergoon::UpdateBattleUI() {
  battleUpdate();
}
