#pragma once
#include <Supergoon/Primitives/Vector2.h>
namespace Supergoon {
struct PlayerSpawnComponent {
  sgVector2 Location = {0, 0};
  int SpawnLocationId;
  int SpawnDirection;
};
} // namespace Supergoon
