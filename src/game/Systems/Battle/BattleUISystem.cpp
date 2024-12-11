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

static Panel* battlePanel;
static GameState* gameState;
static bool initialized = false;

static void createPlayersPanel(UIObject* parent, string name, int hp, int maxHp, int mp, int maxMp) {
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

static void initializeBattleUI() {
	auto fullSize = Point{435, 66};
	battlePanel = CreateUIBasePanel("battleBasePanel", Point{39, 222}, fullSize);
	auto verticalLayoutGroup = new UIVerticalLayoutGroup(battlePanel, "battlersVerticalGroup");
	verticalLayoutGroup->SetLayer(1);
	verticalLayoutGroup->YSpaceBetweenElements = 16;
	verticalLayoutGroup->Offset = {9, 9};
	createPlayersPanel(verticalLayoutGroup, "Kevin", 500, 500, 50, 50);
	createPlayersPanel(verticalLayoutGroup, "Quinn", 100, 100, 10, 20);
	createPlayersPanel(verticalLayoutGroup, "Misha", 300, 300, 100, 100);
	initialized = true;
}

void Supergoon::UpdateBattleUI() {
	if (!gameState) {
		gameState = GameObject::FindComponent<GameState>();
	}
	assert(gameState && "Gamestate not created");
	if (!initialized && gameState->InBattle && !gameState->BattleData.BattleVictory) {
		initializeBattleUI();

	} else if (battlePanel && gameState->BattleData.BattleVictory) {
		Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void*)"battleBasePanel");
		battlePanel = nullptr;
		initialized = false;
	}
}
