#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
struct BattleZoneComponent {
	Rectangle BoundingBox;
	unsigned int BattleId;
	float EncounterTime = 5.0f;
};
}  // namespace Supergoon
