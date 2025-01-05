#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
struct BattleZoneComponent {
  Rectangle BoundingBox = {0, 0, 0, 0};
  unsigned int BattleId = 0;
  float EncounterTime = 5.0f;
};
} // namespace Supergoon
