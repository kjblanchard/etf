#include <Components/TextInteractionComponent.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/TextInteractionSystem.hpp>
using namespace Supergoon;

static GameState* gameStateComponent;
static TextInteractionComponent* currentInteractingText;

void updateTextInteractionComponents(GameObject, TextInteractionComponent& textInteractionComponent) {
	//  This is set when something interacts with it, ie player hits space, if it isn't being interacted with quick return;
	// Also quick return if we are interacting, but the interacting text is not the one who is interacting.
	// if (textInteractionComponent.InteractionPressed) {
	// 	return;
	// }
	if (!textInteractionComponent.InteractionPressed || (currentInteractingText && currentInteractingText != &textInteractionComponent)) {
		return;
	}
	if (gameStateComponent->Interacting) {
		// If we are already engaged in a interaction, then progress it.

		// If we are finished displaying, then we should close and give control back.
		textInteractionComponent.InteractionPressed = false;
		gameStateComponent->Interacting = false;
		currentInteractingText = nullptr;
	} else {
		textInteractionComponent.InteractionPressed = false;
		currentInteractingText = &textInteractionComponent;
		gameStateComponent->Interacting = true;
		// If we are not engaged in an interaction, then we should start it.
	}
}
void Supergoon::InitializeTextInteractionUI() {
	// Create the UI that we use for this.
}

void Supergoon::UpdateTextInteractions() {
	assert(GameObject::FindComponent<GameState>());
	gameStateComponent = GameObject::FindComponent<GameState>();
	GameObject::ForEach<TextInteractionComponent>(updateTextInteractionComponents);
}
