#include <Components/TextInteractionComponent.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/TextInteractionSystem.hpp>
using namespace Supergoon;

void updateTextInteractionComponents(GameObject, TextInteractionComponent textInteractionComponent) {
}

void Supergoon::UpdateTextInteractions() {
	GameObject::ForEach<TextInteractionComponent>(updateTextInteractionComponents);
}
