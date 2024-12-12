#include "SDL3/SDL_filesystem.h"
#include "Supergoon/Primitives/Rectangle.hpp"
#include "Supergoon/UI/UIImage.hpp"
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
#include <Utilities/Utilities.hpp>
#include <sstream>
using namespace Supergoon;
using namespace std;

static Panel *battlePanel;
static Panel *battleCommandPanel;
static UIImage *battleFinger;
static UIText *attackText;
static GameState *gameState;
static bool initialized = false;

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
  auto fullSize = Point{100, 65};
  battleCommandPanel = CreateUIBasePanel("battleCommandPanel", Point{95, 215}, fullSize, 255);
  battleCommandPanel->SetLayer(1);
  auto verticalLayoutGroup = new UIVerticalLayoutGroup(battleCommandPanel, "battleCommandVLG");
  verticalLayoutGroup->SetLayer(2);
  verticalLayoutGroup->YSpaceBetweenElements = 17;
  verticalLayoutGroup->Offset = {18, 9};
  attackText = new UIText(verticalLayoutGroup, "Attack", "attackText", 12);
  new UIText(verticalLayoutGroup, "Magic", "magicText", 12);
  new UIText(verticalLayoutGroup, "Item", "itemText", 12);
  auto path = std::string(SDL_GetBasePath()) + "assets/img/fingers.png";
  auto fingerImage = ContentRegistry::CreateContent<Image>(path);
  fingerImage->LoadContent();
  battleFinger = new UIImage(battleCommandPanel, "battleFinger");
  battleFinger->ImagePtr = fingerImage;
  battleFinger->Bounds.X =
      battleFinger->Bounds.W = 16;
  battleFinger->Bounds.H = 16;
  battleFinger->ImageSourceRect = {16, 48, 16, 16};
  // battleFinger->ImageSourceRect = RectangleF(48, 16, 16, 16);
  battleFinger->SetLayer(3);
}

static void initializeBattleUI() {
  auto fullSize = Point{435, 66};
  battlePanel = CreateUIBasePanel("battleBasePanel", Point{39, 222}, fullSize, 255);
  auto verticalLayoutGroup = new UIVerticalLayoutGroup(battlePanel, "battlersVerticalGroup");
  verticalLayoutGroup->SetLayer(1);
  verticalLayoutGroup->YSpaceBetweenElements = 16;
  verticalLayoutGroup->Offset = {9, 9};
  createPlayersPanel(verticalLayoutGroup, "Kevin", 500, 500, 50, 50);
  createPlayersPanel(verticalLayoutGroup, "Quinn", 100, 100, 10, 20);
  createPlayersPanel(verticalLayoutGroup, "Misha", 300, 300, 100, 100);
  initializePlayerUI();
  initialized = true;
}

void Supergoon::UpdateBattleUI() {
  if (!gameState) {
    gameState = GameObject::FindComponent<GameState>();
  }
  assert(gameState && "Gamestate not created");
  if (!initialized && gameState->InBattle &&
      !gameState->BattleData.BattleVictory) {
    initializeBattleUI();

  } else if (battlePanel && gameState->BattleData.BattleVictory) {
    Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0,
                      (void *)"battleBasePanel");
    battlePanel = nullptr;
    initialized = false;
  }
  if (battlePanel) {
    // TODO this should only happen when we actually move it
    battleFinger->SetDrawOverride({attackText->TextDrawRect.X - 5 - battleFinger->ImageSourceRect.W, attackText->TextDrawRect.Y});
  }
}
