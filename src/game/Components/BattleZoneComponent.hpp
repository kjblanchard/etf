#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
struct BattleZoneComponent {
	Rectangle BoundingBox;
	unsigned int BattleId;
};
}  // namespace Supergoon
