#pragma once
#include <SupergoonEngine/entt/entt.hpp>
#include <optional>

namespace Supergoon {
class GameObject {
   public:
	GameObject(entt::entity e);
	GameObject();
	// Destroy gameobject and all its components
	void FreeGameObject();
	template <typename T, typename... Args>
	// Add a component or components to a gameobject
	void AddComponent(Args&&... args);
	template <typename T>
	// Get a component ref from a gameobject if it has it, otherwise errors
	T& GetComponent();
	template <typename T>
	// Checks if a gameobject has a component
	bool HasComponent() const;
	template <typename T>
	// Removes a component from a gameobject
	void RemoveComponent();
	template <typename T>
	// Find a component on a gameobject from all entities.  A mix of GetGameObject and Get component to save lines, not super efficient.
	static T* FindComponent();
	template <typename... Components, typename Func>
	static void ForEach(Func func);
	template <typename... Components>
	static std::optional<GameObject> GetGameObjectWithComponents();
	static void ClearGameObjects();
	static int NumberGameObjects();

   private:
	static entt::registry _registry;
	entt::entity _entity;
	// Get a entt view of specific gameobjects with components, probably use each and not this though.
	template <typename... Components>
	static auto GetGameObjectsWithComponents();
};
template <typename T>
T* GameObject::FindComponent() {
	auto view = _registry.view<T>();
	if (view.empty()) {
		return nullptr;
	}
	return &GameObject{*view.begin()}.GetComponent<T>();
}

template <typename T, typename... Args>
void GameObject::AddComponent(Args&&... args) {
	_registry.emplace<T>(_entity, std::forward<Args>(args)...);
}
template <typename T>
T& GameObject::GetComponent() {
	return _registry.get<T>(_entity);
}
template <typename T>
bool GameObject::HasComponent() const {
	return _registry.all_of<T>(_entity);
}

template <typename T>
void GameObject::RemoveComponent() {
	_registry.remove<T>(_entity);
}

template <typename... Components>
std::optional<GameObject> GameObject::GetGameObjectWithComponents() {
	auto view = _registry.view<Components...>();
	if (!view.empty()) {
		return GameObject{*view.begin()};
	} else {
		return std::nullopt;
	}
}

template <typename... Components>
auto GameObject::GetGameObjectsWithComponents() {
	return _registry.view<Components...>();
}

template <typename... Components, typename Func>
void GameObject::ForEach(Func func) {
	auto view = GetGameObjectsWithComponents<Components...>();
	view.each([func](auto entity, auto&... components) {
		GameObject gameObject(entity);
		func(gameObject, components...);
	});
}

}  // namespace Supergoon
