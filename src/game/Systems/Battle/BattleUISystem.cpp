#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/UI/UIVerticalLayoutGroup.hpp>
#include <Supergoon/pch.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
#include <Utilities/Utilities.hpp>
using namespace Supergoon;
using namespace std;

void Supergoon::InitializeBattleUI() {
	auto fullSize = Point{435, 66};
	auto panel = CreateUIBasePanel("battleBasePanel", Point{39, 222}, fullSize);
	auto verticalLayoutGroup = new UIVerticalLayoutGroup(panel, "battlersVerticalGroup");
	verticalLayoutGroup->YSpaceBetweenElements = 16;
	verticalLayoutGroup->Offset = {9, 9};
	auto battler1Panel = new Panel(verticalLayoutGroup, "battler1Panel");
	auto horizontalGroup = new UIHorizontalLayoutGroup(battler1Panel, "battler1horiGroup");
	auto nameText = new UIText(horizontalGroup, "Kevin:", "battler1NameDisplayText", 12);
	auto hpText = new UIText(horizontalGroup, "HP: 100|100", "battler1HPDisplayText", 12);
	auto mpTExt = new UIText(horizontalGroup, "MP: 50|50", "battler1MPDisplayText", 12);
	// auto text2 = new UIText(horiGroup, "Misha:    HP: 430 | 430    MP: 50 | 50", "battleDisplayText1", 12);
	// auto text3 = new UIText(horiGroup, "Quinn:    HP: 100 | 100    MP: 50 | 50", "battleDisplayText2", 12);
	// auto text = new UIText(horiGroup, "Kevin:     HP: 500 | 500    MP: 50 | 50", "battleDisplayText", 12);
	// auto text2 = new UIText(horiGroup, "Misha:    HP: 430 | 430    MP: 50 | 50", "battleDisplayText1", 12);
	// auto text3 = new UIText(horiGroup, "Quinn:    HP: 100 | 100    MP: 50 | 50", "battleDisplayText2", 12);
	nameText->SetLayer(1);
	hpText->SetLayer(1);
	mpTExt->SetLayer(1);
}

void Supergoon::UpdateBattleUI() {
}
