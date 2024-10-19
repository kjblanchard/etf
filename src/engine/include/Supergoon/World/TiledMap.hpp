#pragma once
#include <string>
#include <vector>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <variant>

namespace Supergoon {
/**
 * @brief The types of tilesets from tiled that is supported.
 */
enum class TilesetType {
	Default,
	Image,
	Tile,
};

/**
 * @brief A model of a tiledmap json.
 */
class TiledMap {
   public:
	struct TiledProperty {
		std::string Name, PropertyType;
		std::variant<int, std::string, float> Value;
	};
	struct TiledObject {
		std::string Name;
		int Id, Height, Width, X, Y;
		std::string ObjectType;
		std::vector<TiledProperty> Properties;
	};
	struct Tile {
		int Id, ImageHeight, ImageWidth = 0;
		std::string Image;
	};
	struct Tileset {
		TilesetType Type;
		std::vector<Tile> Tiles;
		int Columns, TileHeight, TileWidth, FirstGid = 0;
		std::string Name, Image;
	};

	struct TiledMapTileset {
		int FirstGid = 0;
		std::string Source;
	};
	struct Layer {
		int Height, Width;
		std::string Type, Name;
		std::vector<int> Data;
		std::vector<TiledObject> Objects;
	};
	struct LayerGroup {
		std::string Name;
		std::vector<Layer> Layers;
	};

   public:
	TiledMap(std::string filename);
	RectangleF GetGidSourceRect(int gid);
	const TiledMapTileset *GetGidTiledMapTileset(int gid);
	const Tileset *GetTiledMapTilesetTileset(const TiledMapTileset *tilesetName);

   public:
	int Width, Height, TileWidth, TileHeight;
	// std::string NextLevel;
	std::vector<TiledMapTileset> TiledmapTilesets;
	std::vector<Tileset> Tilesets;
	std::vector<LayerGroup> Groups;
	std::vector<Layer> Layers;
	std::vector<TiledObject> Objects;
	std::vector<TiledObject> SolidObjects;
	std::vector<TiledProperty> Properties;

   private:
	RectangleF GetSourceRectForGidWithTileset(int gid, const TiledMap::Tileset *tileset);
};
}  // namespace Bba
