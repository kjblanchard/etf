#include <Components/BattleLocationComponent.hpp>
#include <Entities/Battle/BattleLocation.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
using namespace Supergoon;

GameObject* Supergoon::NewBattleLocation(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto p = BattleLocationComponent();
	p.Location.X = obj.X;
	p.Location.Y = obj.Y;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "id") {
			p.BattleLocationId = std::get<int>(prop.Value);
		}
	}
	go->AddComponent<BattleLocationComponent>(p);
	return go;
}
