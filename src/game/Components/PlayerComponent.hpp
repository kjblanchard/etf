#pragma once
#include <Supergoon/Physics/Directions.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>

namespace Supergoon {
struct PlayerComponent {
	unsigned int PlayerNum;
	RectangleF Body;
	Directions Direction;
	bool Moving = false;
};

}  // namespace Supergoon
