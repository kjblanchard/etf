#include <Components/Location.hpp>
#include <Supergoon/ECS/CameraComponent.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Systems/DebugDrawSystem.hpp>
using namespace Supergoon;

static void drawRigidBodyDebugBoxes(GameObject, LocationComponent& location, RigidbodyComponent& rb) {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();
	auto dst = RectangleF{location.Location.X - cc.Box.X, location.Location.Y - cc.Box.Y, (float)rb.Size().x, (float)rb.Size().y};
	auto graphics = Graphics::Instance();
	graphics.DrawRect(dst, Color{255, 0, 0, 255});
}

void Supergoon::DrawDebugBoxes() {
	GameObject::ForEach<LocationComponent, RigidbodyComponent>(drawRigidBodyDebugBoxes);
}
