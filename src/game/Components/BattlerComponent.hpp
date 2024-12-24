#pragma once
#include <Entities/Battle/Stats.hpp>
namespace Supergoon {
struct BattlerComponent {
  unsigned int Id = 0;
  int CurrentBattleLocation = 0;
  bool IsPlayer = false;
  float CurrentATB = 0;
  float FullATB = 0;
  Stats Stat = {};
};
} // namespace Supergoon
