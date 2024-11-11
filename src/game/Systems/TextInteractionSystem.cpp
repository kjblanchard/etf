#include <Components/PlayerComponent.hpp>
#include <Components/PlayerInteractionComponent.hpp>
#include <Components/TextInteractionComponent.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/TextInteractionSystem.hpp>
#include <Utilities/Utilities.hpp>
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
		// update the text interaction box to say what this is.
		auto ui = UI::UIInstance;
		auto panelName = "textTesting" + std::string("regular");
		auto thing = (Panel*)ui->Children[panelName].get();
		assert(thing);
		auto textName = "textman" + std::string("regular");
		auto text = (UIText*)thing->Children[textName].get();
		assert(text);
		text->UpdateText(textInteractionComponent.Text);

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
	CreateUITextbox("regular", Point{145, 200}, Point{236, 80});
}

void Supergoon::UpdateTextInteractions() {
	assert(GameObject::FindComponent<GameState>());
	gameStateComponent = GameObject::FindComponent<GameState>();
	GameObject::ForEach<TextInteractionComponent>(updateTextInteractionComponents);
}

void Supergoon::DrawTextInteractionDisplay() {
	GameObject::ForEach<LocationComponent, PlayerInteractionComponent>(drawTextInteractionComponents);
}
