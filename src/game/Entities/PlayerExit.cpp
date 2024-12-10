#include <Components/PlayerExitComponent.hpp>
#include <Entities/PlayerExit.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <variant>
namespace Supergoon {

GameObject* NewPlayerExit(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto p = PlayerExitComponent();
	p.BoundingBox.X = obj.X;
	p.BoundingBox.Y = obj.Y;
	p.BoundingBox.W = obj.Width;
	p.BoundingBox.H = obj.Height;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "loadLocation") {
			p.SpawnLocationId = std::get<int>(prop.Value);
		}
		if (prop.Name == "loadMap") {
			p.NextMap = std::get<std::string>(prop.Value);
		}
	}
	go->AddComponent<PlayerExitComponent>(p);
	return go;
}
}  // namespace Supergoon
