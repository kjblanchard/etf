#include <Components/PlayerComponent.hpp>
#include <Components/PlayerInteractionComponent.hpp>
#include <Components/TextInteractionComponent.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIText.hpp>
// #include <Supergoon/pch.hpp>
#include <Supergoon/ECS/Components/CameraComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/Events.hpp>
#include <Systems/TextInteractionSystem.hpp>
#include <Utilities/Utilities.hpp>
using namespace Supergoon;

static GameState* gameStateComponent;
static TextInteractionComponent* currentInteractingText;
static Tween typingTween = Tween(0);
static bool isTyping = false;

void updateTextInteractionComponents(GameObject, TextInteractionComponent& textInteractionComponent) {
	//  This is set when something interacts with it, ie player hits space, if it isn't being interacted with quick return;
	// Also quick return if we are interacting, but the interacting text is not the one who is interacting.
	// if (textInteractionComponent.InteractionPressed) {
	// 	return;
	// }
	if ((currentInteractingText && currentInteractingText != &textInteractionComponent)) {
		return;
	}
	// If we are already engaged in a interaction, then progress it.
	if (gameStateComponent->Interacting) {
		// If we still have some to type
		if (isTyping) {
			typingTween.Update();
			auto ui = UI::UIInstance.get();
			auto panelName = "textTesting" + std::string("regular");
			auto thing = (Panel*)ui->Children[panelName].get();
			thing->Dirty = true;

			// If we are complete and click
			if (typingTween.Complete()) {
				// If we click, we should end
				isTyping = false;
				Events::PushEvent(Events::BuiltinEvents.StopBgmEvent, 1);
				// If we click and it is not complete, finish typing the text.
			} else if (textInteractionComponent.InteractionPressed) {
				auto textName = "textman" + std::string("regular");
				auto text = (UIText*)thing->Children[textName].get();
				text->SetCurrentLetters(textInteractionComponent.DisplayText.length());
				isTyping = false;
				Events::PushEvent(Events::BuiltinEvents.StopBgmEvent, 1);
			}
			// if we are finished typing and press a button, then we should exit.
		} else {
			if (textInteractionComponent.InteractionPressed) {
				textInteractionComponent.InteractionPressed = false;
				auto ui = UI::UIInstance.get();
				auto panelName = "textTesting" + std::string("regular");
				auto thing = (Panel*)ui->Children[panelName].get();
				thing->SetVisible(false);
				gameStateComponent->Interacting = false;
				currentInteractingText = nullptr;
				isTyping = false;
			}
		}
		// If we aren't interacting, then we should start to
	} else if (textInteractionComponent.InteractionPressed) {
		textInteractionComponent.InteractionPressed = false;
		currentInteractingText = &textInteractionComponent;
		gameStateComponent->Interacting = true;
		// update the text interaction box to say what this is.
		auto ui = UI::UIInstance.get();
		auto panelName = "textTesting" + std::string("regular");
		auto thing = (Panel*)ui->Children[panelName].get();
		assert(thing);
		auto textName = "textman" + std::string("regular");
		auto text = (UIText*)thing->Children[textName].get();
		// auto text = std::dynamic_pointer_cast<UIText>(thing->Children[textName]);
		assert(text);
		// text->TextPtr = textInteractionComponent.TextPtr;
		text->UpdateText(textInteractionComponent.DisplayText);
		text->SetCurrentLetters(0);
		isTyping = true;
		typingTween = Tween(0, textInteractionComponent.DisplayText.length(), textInteractionComponent.DisplayText.length() * 0.05, text->CurrentLettersRef(), Supergoon::Easings::Linear);
		Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 1, (void*)strdup("typing"));
		thing->SetVisible(true);
	}
}

void drawTextInteractionComponents(GameObject, LocationComponent& location, PlayerInteractionComponent& playerInteraction) {
	if (!playerInteraction.ImageShowing) {
		return;
	}
	// auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	// auto& cc = c->GetComponent<CameraComponent>();

	// float adjustedX = location.Location.X + 14 - cc.Box.X;
	// float adjustedY = location.Location.Y - 4 - cc.Box.Y;
	// auto dst = RectangleF{adjustedX, adjustedY, (float)playerInteraction.InteractionImage->Width(), (float)playerInteraction.InteractionImage->Height()};
	// auto src = RectangleF{0, 0, 0, 0};
	// playerInteraction.InteractionImage->Draw(src, dst);
}

void loadTextInteractionComponents(GameObject, TextInteractionComponent& textInteraction) {
	// textInteraction.TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(textInteraction.DisplayText, "commodore", 16);
	auto ui = UI::UIInstance.get();
	auto panelName = "textTesting" + std::string("regular");
	auto thing = (Panel*)ui->Children[panelName].get();
	assert(thing);
	auto textName = "textman" + std::string("regular");
	auto text = (UIText*)thing->Children[textName].get();
	textInteraction.TextPtr = text->TextPtr;
	text->UpdateText(textInteraction.DisplayText);
	text->OnDirty();
}
void Supergoon::InitializeTextInteractionUI() {
	auto textPanel = CreateUITextbox("regular", Point{145, 200}, Point{236, 80}, false);
	textPanel->SetVisible(false);
}

void Supergoon::UpdateTextInteractions() {
	// assert(GameObject::FindComponent<GameState>());
	// gameStateComponent = GameObject::FindComponent<GameState>();
	// GameObject::ForEach<TextInteractionComponent>(updateTextInteractionComponents);
}

void Supergoon::DrawTextInteractionDisplay() {
	// GameObject::ForEach<LocationComponent, PlayerInteractionComponent>(drawTextInteractionComponents);
}
void Supergoon::LoadTextInteractions() {
	// GameObject::ForEach<TextInteractionComponent>(loadTextInteractionComponents);
}
