#include "SDL3/SDL_filesystem.h"
#include "Supergoon/Primitives/Point.h"
#include "Supergoon/Primitives/Rectangle.hpp"
#include "Supergoon/UI/UIImage.hpp"
#include "SupergoonEngine/log.h"
#include "Utilities/Events.hpp"
#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleState.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/UI/UIProgressBar.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
#include <Utilities/Utilities.hpp>
#include <memory>
#include <sstream>
using namespace Supergoon;
using namespace std;
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
  unsigned int BattlerID;
  UIHorizontalLayoutGroup *LayoutGroup;
  UIText *Name;
  UIText *HP;
  UIText *MP;
  // UIText *Speed;
  UIProgressBar *ATBBar;
};
static PlayerUIPanel _playerUIPanels[3];

static void createPlayersPanel(UIObject *parent) {
  for (size_t i = 0; i < 3; i++) {
    _playerUIPanels[i].LayoutGroup = new UIHorizontalLayoutGroup(parent, to_string(i) + "battlerhoriGroup");
    _playerUIPanels[i].LayoutGroup->XSpaceBetweenElements = 110;
    _playerUIPanels[i].Name = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "name", 12);
    _playerUIPanels[i].HP = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "hp", 12);
    _playerUIPanels[i].MP = new UIText(_playerUIPanels[i].LayoutGroup, "", to_string(i) + "mp", 12);
    auto path = std::string(SDL_GetBasePath()) + "assets/img/atbBar.png";
    auto atbBar = ContentRegistry::CreateContent<Image>(path);
    atbBar->LoadContent();
    _playerUIPanels[i].ATBBar = new UIProgressBar(_playerUIPanels[i].LayoutGroup, to_string(i) + "atb", "atbBar");
    _playerUIPanels[i].ATBBar->SetLayer(3);
    _playerUIPanels[i].ATBBar->BarOffset = {9, 13};
    _playerUIPanels[i].ATBBar->BarSize = {46, 4};
    _playerUIPanels[i].ATBBar->ProgressBarColor = {0, 140, 0, 255};

    _playerUIPanels[i].ATBBar->Offset.Y = -6;
    _playerUIPanels[i].ATBBar->ProgressBarImage->Bounds.W = 64;
    _playerUIPanels[i].ATBBar->ProgressBarImage->Bounds.H = 32;
    _playerUIPanels[i].ATBBar->ProgressBarImage->ImageSourceRect = {0, 0, 16, 16};
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
    stringBuilder << comp->Stat.Name << ":	";
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
  battleCommandPanel = CreateUIBasePanel("battleCommandPanel", {95, 215}, fullSize, 255);
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
}

static void startBattleUI() {
  static int num = 0;
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

void Supergoon::InitializeBattleUI() {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleFullyStarted, [](int, void *, void *) {
    startBattleUI();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int, void *, void *) {
    battleCommandPanel->SetVisible(false);
    fingerPosChanged = true;
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.PlayerBattlerTurnBegin, [](int, void *, void *) {
    if (battleCommandPanel && battleFinger) {
      battleCommandPanel->SetVisible(true);
      handleInput(0);
      battleCommandPanel->SetDirty();
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
  initializeBattleUI();
}

void Supergoon::UpdateBattleUI() {
  battleUpdate();
}
