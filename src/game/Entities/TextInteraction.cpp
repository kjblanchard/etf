#include <Supergoon/ECS/GameObject.h>

#include <Components/TextInteractionComponent.hpp>
#include <Entities/TextInteraction.hpp>
#include <string>
#include <variant>
using namespace Supergoon;
GameObject Supergoon::NewTextInteraction(TiledMap::TiledObject& obj) {
	auto go = sgGameObjectCreate();
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
	sgComponentDeclare(TextInteractionComponent);
	sgGameObjectAddComponent(go, TextInteractionComponent, &textInteraction);
	return go;
}
