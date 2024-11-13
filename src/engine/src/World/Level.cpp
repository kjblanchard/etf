#include <SDL3/SDL.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/ECS/Components/CameraComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/KeepAliveComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Components/SolidComponent.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/World/Level.hpp>
#include <algorithm>

namespace Supergoon {
extern std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap;
}
using namespace Supergoon;

std::unique_ptr<Level> Level::_currentLevel = nullptr;
std::function<void()> Level::LoadFunc = nullptr;
void Level::AddLevelEventHandlers() {
	Events::RegisterEventHandler(Events::BuiltinEvents.CameraUpdate, [](int, void *newLoc, void *) {
		assert((RectangleF *)newLoc);
		auto rect = (RectangleF *)newLoc;
		if (!Level::_currentLevel) {
			return;
		}
		Level::_currentLevel->cameraX = rect->X;
		Level::_currentLevel->cameraY = rect->Y;
	});
	Events::RegisterEventHandler(Events::BuiltinEvents.LevelChangeEvent, [](int shouldFade, void *levelName, void *) {
		auto level = (const char *)levelName;
		assert(level);
		if (shouldFade) {
			LoadNewLevelFade(level);
		} else {
			LoadNewLevel(level);
		}
		SDL_free(levelName);
	});
	Events::RegisterEventHandler(Events::BuiltinEvents.GameObjectAdd, [](int, void *gameObject, void *) {
		assert((GameObject *)gameObject);
		_currentLevel->AddGameObjectToLevel((GameObject *)gameObject);
	});
}

std::string Level::GetBgm() {
	auto iterator = std::find_if(_mapData->Properties.begin(), _mapData->Properties.end(), [](TiledMap::TiledProperty &prop) {
		if (prop.Name == "bgm") {
			return true;
		}
		return false;
	});
	if (iterator == _mapData->Properties.end()) {
		return "";
	}
	return std::get<std::string>(iterator->Value);
}

Level::Level(const char *filename)
	: _background(nullptr) {
	_name = filename;
	_mapData = std::make_unique<TiledMap>(filename);
	LoadSurfaces();
	CreateBackgroundImage();
	LoadAllGameObjects();
	LoadSolidObjects();
	auto gamestate = GameObject::FindComponent<GameState>();
	if (!gamestate) {
		// TODO when do we clean this up?  Currently comes up as a leak.
		auto gsGo = new GameObject();
		auto gamestate = GameState();
		auto keepalive = KeepAliveComponent();
		// gamestate.CurrentLevel = this;
		gamestate.PlayerSpawnLocation = 0;
		gamestate.WindowHeight = Graphics::Instance()->LogicalHeight();
		gamestate.WindowWidth = Graphics::Instance()->LogicalWidth();
		gamestate.CameraFollowTarget = true;
		gamestate.Loading = false;
		gsGo->AddComponent<GameState>(gamestate);
		gsGo->AddComponent<KeepAliveComponent>(keepalive);
		AddGameObjectToLevel(gsGo);
	} else {
		// gamestate->CurrentLevel = this;
	}
	auto camGo = new GameObject();
	auto camera = CameraComponent();
	camera.Bounds.X = GetSize().X;
	camera.Bounds.Y = GetSize().Y;
	camera.Box.X = 0;
	camera.Box.Y = 0;
	camGo->AddComponent<CameraComponent>(camera);
	AddGameObjectToLevel(camGo);
}

Level::~Level() {
	// TODO should we actually clear the background testure when level is destroyed here too?
	// TODO when should we clear keepalive components?
	for (auto &&go : _gameObjects) {
		if (go->HasComponent<KeepAliveComponent>()) {
			continue;
		}
		go->FreeGameObject();
		delete (go);
	}
	_gameObjects.clear();
}

static std::string getBasePathForTiled() {
	return std::string(SDL_GetBasePath()) + "assets/tiled/";
}

void Level::Reset() {
	_currentLevel.reset();
}

void Level::LoadSurfaces() {
	for (auto &tileset : _mapData->Tilesets) {
		if (tileset.Type == TilesetType::Image) {
			for (auto &tile : tileset.Tiles) {
				auto fullPath = getBasePathForTiled() + tile.Image;
				_backgroundTilesetImages.push_back(ContentRegistry::CreateContent<Image>(fullPath));
			}
		} else {
			auto fullPath = getBasePathForTiled() + tileset.Image;
			_backgroundTilesetImages.push_back(ContentRegistry::CreateContent<Image>(fullPath));
		}
	}
	ContentRegistry::LoadAllContent();
}

Image *Level::GetSurfaceForGid(int gid, const TiledMap::Tileset *tileset) {
	if (tileset->Type == TilesetType::Image) {
		for (auto &tile : tileset->Tiles) {
			if (tile.Id + tileset->FirstGid == gid) {
				return ContentRegistry::GetContent<Image>(getBasePathForTiled() + tile.Image).get();
			}
		}
	} else {
		auto name = getBasePathForTiled() + tileset->Image;
		return ContentRegistry::GetContent<Image>(getBasePathForTiled() + tileset->Image).get();
	}
	sgLogError("Could not find loaded surface for gid %ud\n", gid);
	return nullptr;
}

void Level::LoadNewLevelFade(std::string level) {
	UI::SetFadeOutEndFunc([level]() {
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, false, (void *)strdup(level.c_str()));
		UI::FadeIn();
	});
	static bool fadebool = true;
	Events::PushEvent(Events::BuiltinEvents.StopBgmEvent, false, (void *)&fadebool);
	UI::FadeOut();
}

void Level::LoadNewLevel(std::string level) {
	_currentLevel = std::make_unique<Level>(level.c_str());
	if (LoadFunc) {
		LoadFunc();
	}
	ContentRegistry::ClearStaleContent();
	auto bgm = _currentLevel->GetBgm();
	auto goboi = GameObject::GetGameObjectWithComponents<GameState>();
	auto &comp = goboi->GetComponent<GameState>();
	comp.Loading = false;
	Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup(bgm.c_str()));
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
	UI::SetFadeOutEndFunc(nullptr);
}

void Level::RestartLevel() {
	CreateBackgroundImage();
	LoadSolidObjects();
	// StartPlayers();
}

void Level::CreateBackgroundImage() {
	if (_background)
		return;
	if (ContentRegistry::ContentExists(_name)) {
		// map is already loaded, so don't blit ot it.
		_background = ContentRegistry::CreateContent<Image, int, int>(_name, _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight);
		return;
	}
	_background = ContentRegistry::CreateContent<Image, int, int>(_name, _mapData->Width * _mapData->TileWidth, _mapData->Height * _mapData->TileHeight);
	_background->SetImageColor(Color{0, 0, 0, 255});
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
					assert(tileSurface);
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
	auto l = LocationComponent();
	auto s = SolidComponent();
	s.Size = Point{t.Width, t.Height};
	l.Location.X = t.X;
	l.Location.Y = t.Y;
	go->AddComponent<SolidComponent>(s);
	go->AddComponent<LocationComponent>(l);
	return go;
}
GameObject *Level::NewSolidObject(Rectangle r) {
	auto go = new GameObject();
	auto l = LocationComponent();
	auto s = SolidComponent();
	s.Size = Point{r.W, r.H};
	l.Location.X = r.X;
	l.Location.Y = r.Y;
	go->AddComponent<SolidComponent>(s);
	go->AddComponent<LocationComponent>(l);
	return go;
}

void Level::LoadSolidObjects() {
	for (auto &solid : _mapData->SolidObjects) {
		auto go = NewSolidObject(solid);
		_gameObjects.push_back(go);
	}
	const int boxSize = 16;
	auto size = GetSize();
	Rectangle top = {0, -boxSize, size.X, boxSize};
	Rectangle right = {size.X, 0, boxSize, size.Y};
	Rectangle bottom = {0, size.Y, size.X, boxSize};
	Rectangle left = {-boxSize, 0, boxSize, size.Y};
	_gameObjects.push_back(NewSolidObject(top));
	_gameObjects.push_back(NewSolidObject(right));
	_gameObjects.push_back(NewSolidObject(bottom));
	_gameObjects.push_back(NewSolidObject(left));
}

void Level::Draw() {
	if (!_currentLevel.get()) {
		return;
	}
	if (_currentLevel->_background) {
		auto screenWidth = Graphics::Instance()->LogicalWidth();
		auto screenHeight = Graphics::Instance()->LogicalHeight();
		auto s = RectangleF();
		auto size = _currentLevel->GetSize();
		s.X = std::round(_currentLevel->cameraX);
		s.Y = std::round(_currentLevel->cameraY);
		s.W = size.X <= screenWidth ? size.X : screenWidth;
		s.H = size.Y <= screenHeight ? size.Y : screenHeight;
		auto d = RectangleF();
		d.X = 0;
		d.Y = 0;
		d.W = s.W;
		d.H = s.H;
		_currentLevel->_background->Draw(s, d);
	}
}

void Level::SetBackGroundColor(Color color) {
	_currentLevel->_background->SetImageColor(color);
}
