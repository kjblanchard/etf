#include <Components/Location.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Systems/LocationSystem.hpp>
using namespace Supergoon;

static void locationEach(GameObject go, LocationComponent& location, RigidbodyComponent rb) {
	location.Location = rb.Location();
}

void Supergoon::UpdateLocationsForRbs() {
	GameObject::ForEach<LocationComponent, RigidbodyComponent>(locationEach);
}
