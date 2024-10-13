#pragma once
#include <SupergoonEngine/prim/point.h>

#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/World/TiledMap.hpp>
#include <memory>
#include <tuple>
namespace Supergoon {
// class Panel;
/**
 * @brief A level that is loaded from tiled, this is loaded from json and contains all tiles, objects, properties, etc
 */
class Level {
   public:
	/**
	 * @brief Construct a new Tiled Level object
	 *
	 * @param filename The file to load from json
	 */
	Level(const char *filename);
	~Level();
	static void LoadNewLevel();
	const inline std::string &GetName() const { return _name; }
	inline gePoint GetSize() { return gePoint{_mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight}; }
	inline void AddGameObjectToLevel(GameObject *g) { _gameObjects.push_back(g); }
	void LoadAllGameObjects();
	// std::vector<TiledMap::TiledObject> GetAllObjects();
	// inline std::vector<TiledMap::TiledObject> GetAllSolidObjects() const { return _mapData->SolidObjects; }
	// Panel *LoadPanel;
	void Draw();
	void RestartLevel();
	void CreateBackgroundImage();
	int cameraX = 0;
	int cameraY = 0;

   private:
	/**
	 * @brief Loads all the surfaces from the tilesets in this level, so that we can blit them properly
	 */
	void LoadSurfaces();
	bool CheckIfTilesetIsCached(const std::string &name) const;
	void LoadSolidObjects();
	/**
	 * @brief Create a Background Atlas object and blits everything to it
	 */
	// /**
	//  * @brief Get the Surface For Gid object
	//  *
	//  * @param gid gid of the tile
	//  * @param tileset the tileset for this tile
	//  * @return struct SDL_Surface* The loaded surface
	//  */
	Image *GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset);
	void StartBgm();

   private:
	std::string _name;
	Image *_background;
	// geImage *_background;
	std::vector<GameObject *> _gameObjects;
	std::unique_ptr<TiledMap> _mapData;
};
}  // namespace Supergoon
