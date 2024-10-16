#include <SDL3/SDL.h>

#include <Supergoon/Aseprite/AsepriteDocument.hpp>
#include <Supergoon/Log.hpp>
#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <istream>
using json = nlohmann::json;
using namespace Supergoon;
AsepriteDocument::AsepriteDocument(std::string p) {
	char *fullPath = NULL;
	SDL_asprintf(&fullPath, "%sassets/aseprite/%s.json", SDL_GetBasePath(), p.c_str());
	std::ifstream fileStream(fullPath);
	auto j = json::parse(fileStream);
	for (const auto &frameJson : j["frames"]) {
		Frame frame;
		frame.filename = frameJson["filename"];
		frame.frame = {
			frameJson["frame"]["x"],
			frameJson["frame"]["y"],
			frameJson["frame"]["w"],
			frameJson["frame"]["h"]};
		frame.rotated = frameJson["rotated"];
		frame.trimmed = frameJson["trimmed"];
		frame.spriteSourceSize = {
			frameJson["spriteSourceSize"]["x"],
			frameJson["spriteSourceSize"]["y"],
			frameJson["spriteSourceSize"]["w"],
			frameJson["spriteSourceSize"]["h"]};
		frame.sourceSize = {
			frameJson["sourceSize"]["w"],
			frameJson["sourceSize"]["h"]};
		frame.duration = frameJson["duration"];
		frames.push_back(frame);
	}
	meta.app = j["meta"]["app"];
	meta.version = j["meta"]["version"];
	meta.image = j["meta"]["image"];
	meta.format = j["meta"]["format"];
	meta.scale = j["meta"]["scale"];
	meta.size = {
		j["meta"]["size"]["w"],
		j["meta"]["size"]["h"]};
	for (const auto &tagJson : j["meta"]["frameTags"]) {
		FrameTag tag;
		tag.name = tagJson["name"];
		tag.direction = tagJson["direction"];
		tag.color = tagJson["color"];
		tag.from = tagJson["from"];
		tag.to = tagJson["to"];
		meta.frameTags.push_back(tag);
	}
	for (const auto &layerJson : j["meta"]["layers"]) {
		Layer layer;
		layer.name = layerJson["name"];
		layer.opacity = layerJson["opacity"];
		layer.blendMode = layerJson["blendMode"];
		meta.layers.push_back(layer);
	}
}
