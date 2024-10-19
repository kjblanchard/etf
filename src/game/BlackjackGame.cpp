#include <Entities/PlayerStart.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>
namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
	{"Start", [](TiledMap::TiledObject &object) {
		 return NewPlayerSpawn(object);
	 }},
};
}
using namespace Supergoon;
Level *level;

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
};

void BlackjackGame::Start() {
	level = new Level("debugTown");
	level->CreateBackgroundImage();
	LoadPlayers();
	LoadAnimationComponents();

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Update() {
	PlayerInput();
	UpdateAnimationComponents();
	UpdateCamera();
}

void BlackjackGame::Draw() {
	level->Draw();
	DrawAnimationComponents();
	DrawImages();

#ifdef imgui
	Widgets::ShowWidgets();
	DrawDebugBoxes();
#endif
}

REGISTER_GAME(BlackjackGame);
