#pragma once
#include <Supergoon/ECS/GameObject.h>

namespace Supergoon {
extern sgComponentDeclare(BattleComponent);
extern sgComponentDeclare(BattleLocationComponent);
extern sgComponentDeclare(BattlerComponent);
extern sgComponentDeclare(BattleZoneComponent);
extern sgComponentDeclare(PlayerExitComponent);
extern sgComponentDeclare(PlayerComponent);
extern sgComponentDeclare(PlayerInteractionComponent);
extern sgComponentDeclare(PlayerSpawnComponent);
extern sgComponentDeclare(SolidObjectComponent);
extern sgComponentDeclare(TextInteractionComponent);
void InitializeComponents();
}  // namespace Supergoon
