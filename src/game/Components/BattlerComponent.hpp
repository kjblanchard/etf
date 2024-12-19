#pragma once
#include <Entities/Battle/Stats.hpp>
namespace Supergoon {
struct BattlerComponent {
  unsigned int Id;
  int CurrentBattleLocation;
  bool IsPlayer;
  float CurrentATB;
  float FullATB;
  Stats Stat;
};
} // namespace Supergoon
