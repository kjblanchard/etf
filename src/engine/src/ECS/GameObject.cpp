#include <Supergoon/ECS/GameObject.hpp>
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
