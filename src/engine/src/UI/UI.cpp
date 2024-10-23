#include <SDL3/SDL.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/UI/ImageObject.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UI.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <fstream>
using namespace Supergoon;
using json = nlohmann::json;

Panel* UI::Initialize() {
	auto rootPanel = new Panel();
	auto graphics = Graphics::Instance();
	rootPanel->Bounds = RectangleF{0, 0, (float)graphics.LogicalWidth(), (float)graphics.LogicalHeight()};
	return rootPanel;
}
void UI::LoadUIFromFile(std::string filename, Panel* rootPanel) {
	// auto rootPanel = new Panel();
	// auto graphics = Graphics::Instance();
	// rootPanel->Bounds = RectangleF{0, 0, (float)graphics.LogicalWidth(), (float)graphics.LogicalHeight()};
	std::string jsonPath = std::string(SDL_GetBasePath()) + "assets/ui/" + filename + ".json";
	std::ifstream fileStream(jsonPath);
	auto j = json::parse(fileStream);
	for (auto& jsonChild : j) {
		if (jsonChild["type"] == "Image") {
			auto name = jsonChild["name"].get<std::string>();
			auto child = std::make_shared<ImageObject>(rootPanel, jsonChild);
			rootPanel->Children[name] = child;
		}
	}
}
