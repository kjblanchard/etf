#include <SDL3/SDL.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/Components/CameraComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/World/Level.hpp>
#include <algorithm>
int SCREEN_WIDTH = 512;
int SCREEN_HEIGHT = 288;

namespace Supergoon {
extern std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;
}
using namespace Supergoon;
static std::vector<std::pair<std::string, Image *>> _imagesCache;

Level::Level(const char *filename)
	: _background(nullptr) {
	_name = filename;
	_mapData = std::make_unique<TiledMap>(filename);
	// _physicsWorld = std::make_unique<PhysicsWorld>();
	LoadSurfaces();
	LoadAllGameObjects();
	LoadSolidObjects();
	// Add gamestate object to level
	auto go = new GameObject();
	auto gamestate = GameState();
	auto camera = CameraComponent();
	camera.Bounds.X = GetSize().X;
	camera.Bounds.Y = GetSize().Y;
	gamestate.CurrentLevel = this;
	gamestate.PlayerSpawnLocation = 0;
	gamestate.WindowHeight = Graphics::Instance().LogicalHeight();
	gamestate.WindowWidth = Graphics::Instance().LogicalWidth();
	go->AddComponent<GameState>(gamestate);
	go->AddComponent<CameraComponent>(camera);
	AddGameObjectToLevel(go);
}

Level::~Level() {
	// Bba::FreeAnimationComponents();
	if (_background) {
		// TODO should we actually clear this?  Save for manual cleanup.
		// geImageFree(_background);
	}
	for (auto &&go : _gameObjects) {
		go->FreeGameObject();
		delete (go);
	}

	_gameObjects.clear();
	// TODO should we actually clear this?  Save for manual cleanup.
	// geBgmDelete(_bgm);
}

void Level::LoadSurfaces() {
	for (auto &tileset : _mapData->Tilesets) {
		if (tileset.Type == TilesetType::Image) {
			for (auto &tile : tileset.Tiles) {
				char *fullPath = NULL;
				SDL_asprintf(&fullPath, "%sassets/tiled/%s", SDL_GetBasePath(), tile.Image.c_str());
				if (!CheckIfTilesetIsCached(fullPath)) {
					auto i = ContentRegistry::CreateContent<Image>(fullPath);
					_imagesCache.push_back({tile.Image, i.get()});
				}
			}
		} else {
			char *fullPath = NULL;
			SDL_asprintf(&fullPath, "%sassets/tiled/%s", SDL_GetBasePath(), tileset.Image.c_str());
			if (!CheckIfTilesetIsCached(fullPath)) {
				auto i = ContentRegistry::CreateContent<Image>(fullPath);
				_imagesCache.push_back({tileset.Image, i.get()});
			}
		}
	}
}

bool Level::CheckIfTilesetIsCached(const std::string &name) const {
	return std::any_of(_imagesCache.begin(), _imagesCache.end(),
					   [&name](const auto &cachedTileset) {
						   return cachedTileset.first == name;
					   });
}

Image *Level::GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset) {
	if (tileset->Type == TilesetType::Image) {
		for (auto &tile : tileset->Tiles) {
			if (tile.Id + tileset->FirstGid == gid) {
				for (auto surface : _imagesCache) {
					if (surface.first == tile.Image)
						return surface.second;
				}
			}
		}
	} else {
		for (auto surface : _imagesCache) {
			if (surface.first == tileset->Image)
				return surface.second;
		}
	}
	printf("Could not find loaded surface for gid %ud\n", gid);
	return nullptr;
}

void Level::LoadNewLevel() {
	// auto l = new Supergoon::Level(Bba::State::NextMapName.c_str());
	// lastLevel = Bba::State::CurrentLevel;
	// Bba::State::CurrentLevel = l;
	// // Load things that potentially the last level would of loaded already
	// l->LoadAllGameObjects();
	// // Create new camera and set bounds
	// auto c = NewCamera();
	// auto &cc = c->GetComponent<CameraComponent>();
	// auto worldSize = l->GetSize();
	// cc.Bounds.x = worldSize.x;
	// cc.Bounds.y = worldSize.y;
	// l->AddGameObjectToLevel(c);
	// l->RestartLevel();
	// if (lastLevel) {
	// 	delete (lastLevel);
	// }
	// // Load things
	// Bba::LoadPlayers();
	// Bba::LoadAnimationComponents();
	// Bba::LoadTextInteractions();
	// // Start
	// l->StartBgm();
	// Bba::StartPlayers();
	// Bba::StartEnemies();
	// Bba::UpdateCamera();
	// // End and fade out
	// State::FadePanel->FadeIn(LevelLoaded);
}

void Level::LoadAllGameObjects() {
	for (auto &obj : _mapData->Objects) {
		auto type = obj.ObjectType;
		auto iter = GameSpawnMap.find(type);
		if (iter == GameSpawnMap.end())
			continue;
		auto go = (*iter).second(obj);
		if (!go) {
			sgLogWarn("Could not create object of type %s", type.c_str());
		}
		_gameObjects.push_back(go);
	}
}

void Level::RestartLevel() {
	CreateBackgroundImage();
	LoadSolidObjects();
	// StartPlayers();
}

void Level::CreateBackgroundImage() {
	if (_background)
		return;
	_background = ContentRegistry::CreateContent<Image, int, int>(_name, _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight);
	ContentRegistry::LoadAllContent();
	for (auto &group : _mapData->Groups) {
		if (group.Name != "background") {
			continue;
		}
		for (auto &groupLayer : group.Layers) {
			for (int y = 0; y < groupLayer.Height; ++y) {
				for (int x = 0; x < groupLayer.Width; ++x) {
					auto index = (y * groupLayer.Width) + x;
					auto tileGid = groupLayer.Data[index];
					if (tileGid == 0)
						continue;
					auto tiledMapTileset = _mapData->GetGidTiledMapTileset(tileGid);
					auto tileset = _mapData->GetTiledMapTilesetTileset(tiledMapTileset);
					auto tileSurface = GetSurfaceForGid(tileGid, tileset);
					auto sourceRect = _mapData->GetGidSourceRect(tileGid);
					auto dstX = x * _mapData->TileWidth;
					auto dstY = y * _mapData->TileHeight;
					// Adjust background image as tiled draws it from the opposite end.
					if (tileset->Type == TilesetType::Image) {
						dstY -= (sourceRect.H - _mapData->TileHeight);
					}
					auto dstRect = RectangleF{(float)dstX, (float)dstY, (float)sourceRect.W, (float)sourceRect.H};
					if (!tileSurface || !_background) {
						continue;
					}
					_background->DrawImageToImage(*tileSurface, sourceRect, dstRect);
				}
			}
		}
	}
}
GameObject *Level::NewSolidObject(TiledMap::TiledObject &t) {
	auto go = new GameObject();
	// auto s = StaticSolidComponent(*this, gePoint{t.Width, t.Height}, Vector2{(float)t.X, (float)t.Y});
	auto l = LocationComponent();
	// l.Location.X = s.Location().X;
	// l.Location.Y = s.Location().Y;
	// go->AddComponent<StaticSolidComponent>(s);
	go->AddComponent<LocationComponent>(l);
	return go;
}

void Level::LoadSolidObjects() {
	for (auto &solid : _mapData->SolidObjects) {
		auto go = NewSolidObject(solid);
		_gameObjects.push_back(go);
	}
	// const int boxSize = 16;
	// auto size = GetSize();
	// geRectangle top = {0, 0, size.x, boxSize};
	// geRectangle right = {size.x + 1, 0, boxSize, size.y};
	// geRectangle bottom = {0, size.y + 1, size.x, boxSize};
	// geRectangle left = {-boxSize, 0, boxSize, size.y};
	// _gameObjects.push_back(NewSolidObject(top));
	// _gameObjects.push_back(NewSolidObject(right));
	// _gameObjects.push_back(NewSolidObject(bottom));
	// _gameObjects.push_back(NewSolidObject(left));
}

void Level::Draw() {
	if (_background) {
		auto s = RectangleF();
		auto size = GetSize();
		s.X = cameraX;
		s.Y = cameraY;
		s.W = size.X <= SCREEN_WIDTH ? size.X : SCREEN_WIDTH;
		s.H = size.Y <= SCREEN_HEIGHT ? size.Y : SCREEN_HEIGHT;
		auto d = RectangleF();
		d.X = 0;
		d.Y = 0;
		d.W = SCREEN_WIDTH;
		d.H = SCREEN_HEIGHT;
		_background->Draw(s, d);
	}
}
