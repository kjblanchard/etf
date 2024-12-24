#pragma once
enum class DamageTypes {
  None,
  Physical,
  Magical,
  Healing,
};

enum class AbilityFlags {
  None = 0,
  All = 1,
  Piercing = 2,
  NonElemental = 4,
};

struct BattleAbility {
  unsigned int ID;
  DamageTypes DamageType;
  unsigned int Flags;
  float Multiplier;
  const char *AnimationName;
  const char *AbilitySFXName;
};
