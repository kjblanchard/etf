#pragma once
#include <Supergoon/Primitives/Vector2.h>
namespace Supergoon {
struct BattleLocationComponent {
  sgVector2 Location = {0, 0};
  int BattleLocationId = 0;
};

} // namespace Supergoon
