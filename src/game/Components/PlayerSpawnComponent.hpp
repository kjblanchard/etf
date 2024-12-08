#pragma once
#include <Supergoon/Primitives/Vector2.hpp>
namespace Supergoon {
struct PlayerSpawnComponent {
	Vector2 Location;
	int SpawnLocationId;
	int SpawnDirection;
};
}  // namespace Supergoon
