#include <Debug/PlayerCollider.hpp>
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
Panel *ui;

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
};

void BlackjackGame::Start() {
	ui = LoadUIFromFile("logos");
	// level = new Level("debugTown");
	// level->CreateBackgroundImage();
	// LoadPlayers();
	// LoadAnimationComponents();

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Update() {
	// PlayerInput();
	// UpdateAnimationComponents();
	// UpdateCamera();
	ui->UpdateInternal();
}

void BlackjackGame::Draw() {
	ui->Draw();
	// level->Draw();
	// DrawAnimationComponents();
	// DrawImages();

#ifdef imgui
	Widgets::ShowWidgets();
	// DrawDebugBoxes();
	// ShowPlayerColliderWindow();
#endif
}

REGISTER_GAME(BlackjackGame);
