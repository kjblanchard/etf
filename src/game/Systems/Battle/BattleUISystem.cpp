#include "SDL3/SDL_filesystem.h"
#include "Supergoon/Primitives/Point.h"
#include "Supergoon/Primitives/Rectangle.hpp"
#include "Supergoon/UI/UIImage.hpp"
#include "SupergoonEngine/log.h"
#include "Utilities/Events.hpp"
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
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
#include <Utilities/Utilities.hpp>
#include <memory>
#include <sstream>
using namespace Supergoon;
using namespace std;
static const int battleCommandsSize = 3;
static const char *battleCommands[] = {"Attack", "Magic", "Item"};
static Panel *battlePanel;
static Panel *battleCommandPanel;
static UIImage *battleFinger;
static UIText *battleCommandTexts[battleCommandsSize];
static Sequence fingerSequence;
static bool fingerPosChanged = false;

static void createPlayersPanel(UIObject *parent, string name, int hp, int maxHp,
                               int mp, int maxMp) {
  auto horizontalGroup = new UIHorizontalLayoutGroup(parent, name + "horiGroup");
  horizontalGroup->XSpaceBetweenElements = 90;
  ostringstream stringBuilder;
  stringBuilder << name << ":	";
  new UIText(horizontalGroup, stringBuilder.str(), name + "NameDisplayText", 12);
  stringBuilder.str("");
  stringBuilder << "HP: " << hp << " | " << maxHp;
  new UIText(horizontalGroup, stringBuilder.str(), name + "HPDisplayText", 12);
  stringBuilder.str("");
  stringBuilder << "MP: " << mp << " | " << maxMp;
  new UIText(horizontalGroup, stringBuilder.str(), name + "MPDisplayText", 12);
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
}

static void initializeFinger() {
  UI::UIInstance->UpdateInternal();
  fingerPosChanged = true;
}

static void initializeBattleUI() {
  auto fullSize = sgPoint{435, 66};
  battlePanel = CreateUIBasePanel("battleBasePanel", {39, 222}, fullSize, 255);
  auto verticalLayoutGroup = new UIVerticalLayoutGroup(battlePanel, "battlersVerticalGroup");
  verticalLayoutGroup->SetLayer(1);
  verticalLayoutGroup->YSpaceBetweenElements = 16;
  verticalLayoutGroup->Offset = {9, 9};
  createPlayersPanel(verticalLayoutGroup, "Kevin", 500, 500, 50, 50);
  // createPlayersPanel(verticalLayoutGroup, "Quinn", 100, 100, 10, 20);
  // createPlayersPanel(verticalLayoutGroup, "Misha", 300, 300, 100, 100);
  initializePlayerUI();
  initializeFinger();
}

static void handleInput(int loc) {
  sgLogWarn("Moving to finger pos %d", loc);
  auto currentFingerPos = loc;
  auto text = battleCommandTexts[currentFingerPos];
  auto x = text->TextDrawRect.X - 5 - battleFinger->ImageSourceRect.W;
  auto y = text->TextDrawRect.Y;
  battleFinger->SetDrawOverride({*battleFinger->DrawOverrideXHandle(), y});
  text->SetDirty();
  UI::UIInstance->UpdateInternal();
  float *xHandle = &battleFinger->DrawOverrideHandle()->X;
  fingerSequence = Sequence();
  auto fingerbackTween = new Tween(x, x - 5, 1.0, xHandle, Easings::Linear, -1);
  auto fingerForwardTween = new Tween(x - 5, x, 1.0, xHandle, Easings::Linear, -1);
  fingerForwardTween->UpdateFunc = []() {
    battleFinger->SetDirty();
  };
  fingerbackTween->UpdateFunc = []() {
    battleFinger->SetDirty();
  };
  fingerSequence.Tweens.push_back(shared_ptr<Tween>(fingerForwardTween));
  fingerSequence.Tweens.push_back(shared_ptr<Tween>(fingerbackTween));
}

static void battleUpdate() {
  if (!battlePanel) {
    return;
  }
  fingerSequence.Update();
  if (fingerSequence.IsComplete()) {
    fingerSequence.Restart();
  };
}

static void battleVictory() {
  Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"battleBasePanel");
  Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"battleCommandPanel");
  battlePanel = nullptr;
  for (int i = 0; i < battleCommandsSize; ++i) {
    battleCommandTexts[i] = nullptr;
  }
}

static void battleCleanup() {
  fingerPosChanged = false;
}

void Supergoon::InitializeBattleUI() {
  Events::RegisterEventHandler(EscapeTheFateEvents.EnterBattleFinished, [](int, void *, void *) {
    initializeBattleUI();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleTurnFinished, [](int, void *, void *) {
    battleCommandPanel->SetVisible(false);
    fingerPosChanged = true;
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.PlayerBattlerTurnBegin, [](int, void *, void *) {
    handleInput(0);
    battleCommandPanel->SetVisible(true);
    battleCommandPanel->SetDirty();
  });
  Events::RegisterEventHandler(EscapeTheFateEvents.commandCursorUpdate, [](int buttonLoc, void *, void *) {
    if (!battlePanel) {
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
}

void Supergoon::UpdateBattleUI() {
  battleUpdate();
}
