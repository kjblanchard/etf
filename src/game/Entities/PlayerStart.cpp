#include <Supergoon/ECS/Gameobject.h>

#include <Components/PlayerSpawnComponent.hpp>
#include <Entities/PlayerStart.hpp>
using namespace Supergoon;

GameObject Supergoon::NewPlayerSpawn(TiledMap::TiledObject& obj) {
	auto go = sgGameObjectCreate();
	auto p = PlayerSpawnComponent();
	p.Location.X = obj.X;
	p.Location.Y = obj.Y;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "loadLocation") {
			p.SpawnLocationId = std::get<int>(prop.Value);
		}
		if (prop.Name == "direction") {
			p.SpawnDirection = std::get<int>(prop.Value);
		}
	}
	sgComponentDeclare(PlayerSpawnComponent);
	sgGameObjectAddComponent(go, PlayerSpawnComponent, &p);
	return go;
}
