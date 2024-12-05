#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Utilities.hpp>
using namespace Supergoon;

void Supergoon::InitializeBattleUI() {
	auto fullSize = Point{435, 66};
	auto panel = CreateUIBasePanel("battleBasePanel", Point{45, 222}, fullSize);
	auto text = new UIText(panel, "Kevin:     HP: 500 | 500    MP: 50 | 50", "battleDisplayText", 8);
	text->SetLayer(1);
	text->Offset = {6, 6};
	text->Bounds.W = fullSize.X - (text->Offset.X * 2);
	text->Bounds.H = fullSize.Y - (text->Offset.Y * 2);
	text->SetCenterY(true);
}

void Supergoon::UpdateBattleUI() {
}
