#include <SDL3/SDL.h>

// #include <Supergoon/Primitives/Point.hpp>
#include <Supergoon/World/TiledMap.hpp>
#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <istream>
using json = nlohmann::json;
using namespace Supergoon;

const TiledMap::TiledMapTileset *TiledMap::GetGidTiledMapTileset(int gid) {
	TiledMap::TiledMapTileset *tiledMapTileset = nullptr;
	for (auto tileset = TiledmapTilesets.begin(); tileset != TiledmapTilesets.end(); ++tileset) {
		if (gid >= tileset->FirstGid) {
			tiledMapTileset = &(*tileset);
		} else
			break;
	}
	if (!tiledMapTileset) {
		printf("Could not find tileset for git %ud\n", gid);
		return nullptr;
	}
	return tiledMapTileset;
}

const TiledMap::Tileset *TiledMap::GetTiledMapTilesetTileset(const TiledMapTileset *tiledMapTileset) {
	for (auto tileset = Tilesets.begin(); tileset != Tilesets.end(); ++tileset) {
		auto fullname = tileset->Name + ".tsj";
		if (tiledMapTileset->Source == fullname)
			return &(*tileset);
	}
	return nullptr;
}

RectangleF TiledMap::GetSourceRectForGidWithTileset(int gid, const TiledMap::Tileset *tileset) {
	if (tileset->Type == TilesetType::Image) {
		for (auto &tile : tileset->Tiles) {
			if (tile.Id + tileset->FirstGid == gid) {
				return RectangleF{0, 0, (float)tile.ImageWidth, (float)tile.ImageHeight};
			}
		}
	} else {
		// Subtract 1 from gid, since 0 is counted as nothing in tiled.
		// --gid;
		auto tilemapGid = gid - tileset->FirstGid;
		int x = (tilemapGid % tileset->Columns) * tileset->TileWidth;
		int y = (tilemapGid / tileset->Columns) * tileset->TileHeight;
		return RectangleF{(float)x, (float)y, (float)tileset->TileWidth, (float)tileset->TileHeight};
	}
	return RectangleF{0, 0, 0, 0};
}
RectangleF TiledMap::GetGidSourceRect(int gid) {
	auto tiledMapTileset = GetGidTiledMapTileset(gid);
	auto tileset = GetTiledMapTilesetTileset(tiledMapTileset);
	return GetSourceRectForGidWithTileset(gid, tileset);
	// return GetSurfaceForGid(gid, )
}

TiledMap::TiledMap(std::string filename) {
	// auto pathPrefix = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + filename + ".tmj";
	char *fullPath = NULL;
	SDL_asprintf(&fullPath, "%sassets/tiled/%s%s", SDL_GetBasePath(), filename.c_str(), ".tmj");
	// geGetLoadFilename(buf, sizeof(buf), pathPrefix.c_str());
	std::ifstream file(fullPath);
	SDL_free(fullPath);
	json data = json::parse(file);
	// auto &tilesets = data["tilesets"];
	Width = data["width"];
	Height = data["height"];
	TileWidth = data["tilewidth"];
	TileHeight = data["tileheight"];
	for (auto &tileset : data["tilesets"]) {
		auto tilemapTileset = TiledMapTileset();
		tilemapTileset.FirstGid = tileset["firstgid"];
		tilemapTileset.Source = tileset["source"];
		TiledmapTilesets.push_back(tilemapTileset);
	}
	for (auto &tiledmapTileset : TiledmapTilesets) {
		auto tileset = Tileset();
		tileset.FirstGid = tiledmapTileset.FirstGid;
		// std::string tilesetPrefix = ASSET_PREFIX + '/' + TILED_PREFIX + '/' + tiledmapTileset.Source;
		SDL_asprintf(&fullPath, "%sassets/tiled/%s", SDL_GetBasePath(), tiledmapTileset.Source.c_str());
		// geGetLoadFilename(fullPath, sizeof(buf), tilesetPrefix.c_str());
		std::ifstream file(fullPath);
		json tilesetData = json::parse(file);
		// read the tileset file, and load info about it here.
		tileset.Name = tilesetData["name"];
		tileset.TileHeight = tilesetData["tileheight"];
		tileset.TileWidth = tilesetData["tilewidth"];
		tileset.Columns = tilesetData["columns"];
		auto tilesetTilesIterator = tilesetData.find("tiles");
		if (tilesetTilesIterator != tilesetData.end()) {
			tileset.Type = TilesetType::Image;
			for (auto &tilesetTileJson : *tilesetTilesIterator) {
				auto tile = Tile();
				tile.Id = tilesetTileJson["id"];
				tile.ImageWidth = tilesetTileJson["imagewidth"];
				tile.ImageHeight = tilesetTileJson["imageheight"];
				tile.Image = tilesetTileJson["image"];
				tileset.Tiles.push_back(tile);
			}
		} else {
			tileset.Type = TilesetType::Tile;
			tileset.Image = tilesetData["image"];
		}
		Tilesets.push_back(tileset);
	}

	for (auto &layerJson : data["layers"]) {
		std::string layerType = layerJson["type"];
		if (layerType == "group") {
			auto layerGroup = LayerGroup();
			layerGroup.Name = layerJson["name"];
			for (auto &groupLayer : layerJson["layers"]) {
				auto layerGroupLayer = Layer();
				layerGroupLayer.Name = groupLayer["name"];
				layerGroupLayer.Height = groupLayer["height"];
				layerGroupLayer.Width = groupLayer["width"];
				layerGroupLayer.Data = groupLayer["data"].get<std::vector<int>>();
				layerGroup.Layers.push_back(layerGroupLayer);
			}
			Groups.push_back(layerGroup);
		} else if (layerType == "tilelayer") {
			auto layer = Layer();
			layer.Name = layerJson["name"];
			layer.Height = layerJson["height"];
			layer.Width = layerJson["width"];
			layer.Data = layerJson["data"].get<std::vector<int>>();
			Layers.push_back(layer);
		} else if (layerType == "objectgroup") {
			auto layer = Layer();
			auto name = layerJson["name"];
			if (name == "entities") {
				for (auto &objectJson : layerJson["objects"]) {
					auto object = TiledObject();
					object.Name = objectJson["name"];
					object.Id = objectJson["id"];
					object.Height = objectJson["height"];
					object.Width = objectJson["width"];
					object.ObjectType = objectJson["type"];
					object.X = objectJson["x"];
					object.Y = objectJson["y"];
					for (auto &property : objectJson["properties"]) {
						// TODO use union in property class
						auto prop = TiledProperty();
						prop.Name = property["name"];
						prop.PropertyType = property["type"];
						if (prop.PropertyType == "int") {
							prop.Value = property["value"].get<int>();

						} else if (prop.PropertyType == "string") {
							prop.Value = property["value"].get<std::string>();
						}

						object.Properties.push_back(prop);
					}
					Objects.push_back(object);
				}
				Layers.push_back(layer);
			} else if (name == "solid") {
				for (auto &objectJson : layerJson["objects"]) {
					auto object = TiledObject();
					object.Id = objectJson["id"];
					object.X = objectJson["x"];
					object.Y = objectJson["y"];
					object.Width = objectJson["width"];
					object.Height = objectJson["height"];
					SolidObjects.push_back(object);
				}
			}
		}
	}
	for (auto &propertyJson : data["properties"]) {
		TiledProperty property;
		property.Name = propertyJson["name"];
		if (propertyJson.contains("propertytype")) {
			property.PropertyType = propertyJson["propertytype"];
			property.Value = propertyJson["value"].dump();
		} else {
			property.PropertyType = propertyJson["type"];
			property.Value = propertyJson["value"].get<std::string>();
		}
		Properties.push_back(property);
	}
}
