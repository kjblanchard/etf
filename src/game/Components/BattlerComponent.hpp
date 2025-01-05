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
inline bool IsBattlerDead(BattlerComponent &battler) { return battler.Stat.HP <= 0; }
} // namespace Supergoon
