
#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <string>
namespace Supergoon {
struct PlayerExitComponent {
	RectangleF BoundingBox;
	int SpawnLocationId;
	std::string NextMap;
};
}  // namespace Supergoon
