#include <Supergoon/pch.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Utilities.hpp>
#include <Supergoon/UI/UIHorizontalLayoutGroup.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <UI/Battle/BattlerDisplay.hpp>
using namespace Supergoon;
using namespace std;

void Supergoon::InitializeBattleUI() {
	auto fullSize = Point{435, 66};
	auto panel = CreateUIBasePanel("battleBasePanel", Point{39, 222}, fullSize);
	auto horiGroup = new UIHorizontalLayoutGroup(panel, "battlerHoriGroup");
	horiGroup->YSpaceBetweenElements = 16;
	horiGroup->Offset = {9, 9};
	auto
	auto text = new UIText(horiGroup, "Kevin:     HP: 500 | 500    MP: 50 | 50", "battleDisplayText", 12);
	auto text2 = new UIText(horiGroup, "Misha:    HP: 430 | 430    MP: 50 | 50", "battleDisplayText1", 12);
	auto text3 = new UIText(horiGroup, "Quinn:    HP: 100 | 100    MP: 50 | 50", "battleDisplayText2", 12);
	// auto text = new UIText(horiGroup, "Kevin:     HP: 500 | 500    MP: 50 | 50", "battleDisplayText", 12);
	// auto text2 = new UIText(horiGroup, "Misha:    HP: 430 | 430    MP: 50 | 50", "battleDisplayText1", 12);
	// auto text3 = new UIText(horiGroup, "Quinn:    HP: 100 | 100    MP: 50 | 50", "battleDisplayText2", 12);
	text->SetLayer(1);
	text2->SetLayer(1);
	text3->SetLayer(1);
	// text->Bounds.W = fullSize.X - (text->Offset.X * 2);
	// text->Bounds.H = fullSize.Y - (text->Offset.Y * 2);
	// text->SetCenterY(true);
}

void Supergoon::UpdateBattleUI() {
}
