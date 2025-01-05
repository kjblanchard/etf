#include <Components/BattleZoneComponent.hpp>
#include <Entities/Battle/BattleZone.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
using namespace Supergoon;

GameObject* Supergoon::NewBattleZone(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
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
	go->AddComponent<BattleZoneComponent>(battleZone);
	return go;
}
