#include <Supergoon/ECS/Gameobject.hpp>
using namespace Supergoon;

entt::registry GameObject::_registry;

GameObject::GameObject(entt::entity e) : _entity(e) {
}

void GameObject::FreeGameObject() {
	_registry.destroy(_entity);
}

GameObject::GameObject() {
	_entity = _registry.create();
}
void GameObject::ClearGameObjects() {
	_registry.clear();
}

int GameObject::NumberGameObjects() {
	auto count = 0;
	auto view = _registry.view<entt::entity>();
	for (auto &&i : view) {
		(void)i;
		count++;
	}
	return count;
}
