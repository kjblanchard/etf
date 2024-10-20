#pragma once
#include <Supergoon/Physics/Directions.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
inline Directions GetOverlapDirectionF(RectangleF* obj, RectangleF* overlapBox) {
	if (overlapBox->W < overlapBox->H) {
		if (overlapBox->X > obj->X) {
			return Directions::East;
		}
		return Directions::West;
	}
	if (overlapBox->Y > obj->Y) {
		return Directions::South;
	}
	return Directions::North;
}

inline Directions GetOverlapDirection(Rectangle* obj, Rectangle* overlapBox) {
	if (overlapBox->W < overlapBox->H) {
		if (overlapBox->X > obj->X) {
			return Directions::East;
		}
		return Directions::West;
	}
	if (overlapBox->Y > obj->Y) {
		return Directions::South;
	}
	return Directions::North;
}

}  // namespace Supergoon
