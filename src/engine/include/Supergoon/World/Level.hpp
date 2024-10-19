#pragma once
#include <Supergoon/Primitives/Point.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/GameObject.hpp>
#include <Supergoon/Physics/Physics.hpp>
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
	inline Point GetSize() { return Point{_mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight}; }
	inline void AddGameObjectToLevel(GameObject *g) { _gameObjects.push_back(g); }
	void LoadAllGameObjects();
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
	Image *GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset);
	void StartBgm();
	GameObject *NewSolidObject(TiledMap::TiledObject &);

   private:
	std::string _name;
	std::shared_ptr<Image> _background;
	std::vector<GameObject *> _gameObjects;
	std::unique_ptr<TiledMap> _mapData;
};
}  // namespace Supergoon
