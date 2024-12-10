#include <Supergoon/ECS/GameObject.h>

#include <Components/Components.hpp>
using namespace Supergoon;

sgComponentDeclare(BattleComponent);
sgComponentDeclare(BattleLocationComponent);
sgComponentDeclare(BattlerComponent);
sgComponentDeclare(BattleZoneComponent);
sgComponentDeclare(PlayerComponent);
sgComponentDeclare(PlayerExitComponent);
sgComponentDeclare(PlayerInteractionComponent);
sgComponentDeclare(PlayerSpawnComponent);
sgComponentDeclare(SolidObjectComponent);
sgComponentDeclare(TextInteractionComponent);

void Supergoon::InitializeComponents() {
	sgComponentRegister(BattleComponent);
	sgComponentRegister(BattleLocationComponent);
	sgComponentRegister(BattlerComponent);
	sgComponentRegister(BattleZoneComponent);
	sgComponentRegister(PlayerComponent);
	sgComponentRegister(PlayerExitComponent);
	sgComponentRegister(PlayerInteractionComponent);
	sgComponentRegister(PlayerSpawnComponent);
	sgComponentRegister(SolidObjectComponent);
	sgComponentRegister(TextInteractionComponent);
}
