#pragma once
namespace Supergoon {
void InitializeBattleDamageSystem(BattleComponent *battleComponent);
void UpdateBattleDamageSystem();
// Used to end the battle damage system at end of battle.
void EndBattleDamageSystem();
} // namespace Supergoon
