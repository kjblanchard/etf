#include <Supergoon/UI/Panel.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Utilities/Utilities.hpp>
using namespace Supergoon;

void Supergoon::InitializeBattleUI() {
	auto panel = CreateUIBasePanel("battleBasePanel", Point{45, 222}, Point{435, 66});
}

void Supergoon::UpdateBattleUI() {
}
