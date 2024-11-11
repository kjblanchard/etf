#include <Components/PlayerComponent.hpp>
#include <Components/PlayerInteractionComponent.hpp>
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

void drawTextInteractionComponents(GameObject, LocationComponent& location, PlayerInteractionComponent& playerInteraction) {
	if (!playerInteraction.ImageShowing) {
		return;
	}
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();

	float adjustedX = location.Location.X + 14 - cc.Box.X;
	float adjustedY = location.Location.Y - 4 - cc.Box.Y;
	auto dst = RectangleF{adjustedX, adjustedY, (float)playerInteraction.InteractionImage->Width(), (float)playerInteraction.InteractionImage->Height()};
	auto src = RectangleF{0, 0, 0, 0};
	playerInteraction.InteractionImage->Draw(src, dst);
}
void Supergoon::InitializeTextInteractionUI() {
	// Create the UI that we use for this.
}

void Supergoon::UpdateTextInteractions() {
	assert(GameObject::FindComponent<GameState>());
	gameStateComponent = GameObject::FindComponent<GameState>();
	GameObject::ForEach<TextInteractionComponent>(updateTextInteractionComponents);
}

void Supergoon::DrawTextInteractionDisplay() {
	GameObject::ForEach<LocationComponent, PlayerInteractionComponent>(drawTextInteractionComponents);
}
