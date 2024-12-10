#include <Components/TextInteractionComponent.hpp>
#include <Entities/TextInteraction.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <string>
#include <variant>
using namespace Supergoon;
GameObject* Supergoon::NewTextInteraction(TiledMap::TiledObject& obj) {
	auto go = new GameObject();
	auto textInteraction = TextInteractionComponent();
	textInteraction.InteractionRect.X = obj.X;
	textInteraction.InteractionRect.Y = obj.Y;
	textInteraction.InteractionRect.W = obj.Width;
	textInteraction.InteractionRect.H = obj.Height;
	// textInteraction.TextPtr = nullptr;
	for (auto&& prop : obj.Properties) {
		if (prop.Name == "text") {
			textInteraction.DisplayText = std::get<std::string>(prop.Value);
		}
	}
	go->AddComponent<TextInteractionComponent>(textInteraction);
	return go;
}
