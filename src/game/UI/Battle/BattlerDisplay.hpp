#pragma once
#include <string>
namespace Supergoon {
class Panel;
struct BattlerDisplayArgs {
	std::string Name;
	int HP;
	int HPMax;
	int MP;
	int MPMax;
};
Panel* CreateBattlerDisplay(const BattlerDisplayArgs& args);
void UpdateBattlerDisplay(Panel* battlerDisplay);

}  // namespace Supergoon
