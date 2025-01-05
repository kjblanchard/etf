
#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <string>
namespace Supergoon {
struct PlayerExitComponent {
  RectangleF BoundingBox = {0, 0, 0, 0};
  int SpawnLocationId = 0;
  std::string NextMap = "";
};
} // namespace Supergoon
