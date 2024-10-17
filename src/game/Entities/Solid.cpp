#include <Entities/Solid.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/Physics/StaticObjectComponent.hpp>
using namespace Supergoon;

GameObject* Supergoon::NewSolidObject(RectangleF& r) {
	// auto go = new GameObject();
	// auto s = StaticSolidComponent();
	// s.Rectangle.X = r.X;
	// s.Rectangle.Y = r.Y;
	// s.Rectangle.W = r.W;
	// s.Rectangle.H = r.H;
	// auto l = LocationComponent();
	// l.Location.X = s.Rectangle.X;
	// l.Location.Y = s.Rectangle.Y;
	// go->AddComponent<SolidObjectComponent>(s);
	// go->AddComponent<LocationComponent>(l);
	// return go;
}

GameObject* Supergoon::NewSolidObject(TiledMap::TiledObject& t) {
	auto go = new GameObject();
	auto state = GameObject::GetGameObjectWithComponents<GameState>();
	if (!state.has_value()) {
		return nullptr;
	}
	auto& stateComponent = state->GetComponent<GameState>();
	auto s = StaticSolidComponent(*stateComponent.CurrentLevel, gePoint{t.Width, t.Height}, Vector2{(float)t.X, (float)t.Y});
	auto l = LocationComponent();
	l.Location.X = s.Location().X;
	l.Location.Y = s.Location().Y;
	go->AddComponent<StaticSolidComponent>(s);
	go->AddComponent<LocationComponent>(l);
	return go;
}
