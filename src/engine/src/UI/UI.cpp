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

Panel* Supergoon::LoadUIFromFile(std::string filename) {
	auto rootPanel = new Panel();
	auto graphics = Graphics::Instance();
	rootPanel->Bounds = RectangleF{0, 0, (float)graphics.LogicalWidth(), (float)graphics.LogicalHeight()};
	char* jsonPath = NULL;
	SDL_asprintf(&jsonPath, "%sassets/ui/%s.json", SDL_GetBasePath(), filename.c_str());
	std::ifstream fileStream(jsonPath);
	SDL_free(jsonPath);
	auto j = json::parse(fileStream);
	for (auto&& jsonChild : j) {
		if (jsonChild["type"] == "Image") {
			auto child = std::make_shared<ImageObject>();
			child->Visible = jsonChild["visible"].get<bool>();
			auto name = jsonChild["name"].get<std::string>();
			auto sourceData = jsonChild["source"];
			auto destinationData = jsonChild["destination"];
			auto imageName = sourceData["name"].get<std::string>();
			char* fullPath = NULL;
			SDL_asprintf(&fullPath, "%sassets/img/%s%s", SDL_GetBasePath(), imageName.c_str(), ".png");
			child->Image = ContentRegistry::CreateContent<Image>(fullPath);
			child->Image->Load();
			// TODO how do we free this image name
			// sdl_free(fullPath)
			child->Bounds = RectangleF{
				destinationData["x"].get<float>() + rootPanel->Bounds.X,
				destinationData["y"].get<float>() + rootPanel->Bounds.Y,
				destinationData["width"].get<float>(),
				destinationData["height"].get<float>()};
			float x = sourceData["x"].get<float>();
			float y = sourceData["y"].get<float>();
			float w = sourceData["width"].get<float>();
			float h = sourceData["height"].get<float>();
			// Set the source rect, if it is all 0 use the full image.
			if (x == 0 && y == 0 && w == 0 && h == 0) {
				w = child->Image->Width();
				h = child->Image->Height();
			}
			child->ImageSourceRect = RectangleF{x, y, w, h};
			rootPanel->Children[name] = child;
		}
	}
	return rootPanel;
}
