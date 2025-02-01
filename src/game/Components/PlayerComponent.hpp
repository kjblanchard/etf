#pragma once
#include <Supergoon/Physics/Directions.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>

namespace Supergoon {
struct PlayerComponent {
  unsigned int PlayerNum = 0;
  RectangleF Body = {0, 0, 0, 0};
  Directions Direction = Directions::North;
  bool Moving = false;
};

} // namespace Supergoon
