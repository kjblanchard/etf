#include <Supergoon/ECS/GameObject.h>

#include <Components/BattleLocationComponent.hpp>
#include <Entities/Battle/BattleLocation.hpp>
using namespace Supergoon;

GameObject Supergoon::NewBattleLocation(TiledMap::TiledObject& obj) {
	auto go = sgGameObjectCreate();
	auto id = 0;
	auto vec = Vector2(obj.X, obj.Y);
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "id") {
			id = std::get<int>(prop.Value);
		}
	}
	BattleLocationComponent blc = {vec, id};
	sgComponentDeclare(BattleLocationComponent);
	sgGameObjectAddComponent(go, BattleLocationComponent, &blc);
	return go;
}
