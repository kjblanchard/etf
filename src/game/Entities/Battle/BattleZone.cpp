#include <Supergoon/ECS/GameObject.h>

#include <Components/BattleZoneComponent.hpp>
#include <Entities/Battle/BattleZone.hpp>
using namespace Supergoon;

GameObject Supergoon::NewBattleZone(TiledMap::TiledObject& obj) {
	auto go = sgGameObjectCreate();
	auto battleZone = BattleZoneComponent();
	battleZone.BoundingBox.X = obj.X;
	battleZone.BoundingBox.Y = obj.Y;
	battleZone.BoundingBox.W = obj.Width;
	battleZone.BoundingBox.H = obj.Height;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "battleId") {
			battleZone.BattleId = 1;
		}
		if (prop.Name == "encounterTime") {
			battleZone.EncounterTime = std::get<float>(prop.Value);
		}
	}
	sgComponentDeclare(BattleZoneComponent);
	sgGameObjectAddComponent(go, BattleZoneComponent, &battleZone);
	return go;
}
