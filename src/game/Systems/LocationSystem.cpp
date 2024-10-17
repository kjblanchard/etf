#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/Physics/StaticObjectComponent.hpp>
#include <Systems/LocationSystem.hpp>
using namespace Supergoon;

// static void locationEach(GameObject go, LocationComponent& location, RigidbodyComponent rb) {
// 	location.Location = rb.Location();
// }
static void locationEachStatic(GameObject go, LocationComponent& location, StaticSolidComponent rb) {
	location.Location = rb.Location();
}

void Supergoon::UpdateLocationsForRbs() {
	// GameObject::ForEach<LocationComponent, RigidbodyComponent>(locationEach);
	// GameObject::ForEach<LocationComponent, StaticSolidComponent>(locationEachStatic);
}
