#include <Components/PlayerSpawnComponent.hpp>
#include <Entities/PlayerStart.hpp>
#include <Supergoon/ECS/GameObject.hpp>
// #include <BbAdventures/base/GameObject.hpp>
// #include <BbAdventures/components/PlayerSpawnComponent.hpp>
// #include <BbAdventures/entities/PlayerStart.hpp>
// #include <BbAdventures/shared/state.hpp>
// namespace Bba {
using namespace Supergoon;

GameObject* Supergoon::NewPlayerSpawn(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto p = PlayerSpawnComponent();
	p.Location.X = obj.X;
	p.Location.Y = obj.Y;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "loadLocation") {
			p.SpawnLocationId = std::get<int>(prop.Value);
		}
		// if (prop.Name == "direction") {
		// 	p.SpawnDirection = (Directions)std::get<int>(prop.Value);
		// }
	}
	go->AddComponent<PlayerSpawnComponent>(p);
	return go;
}
