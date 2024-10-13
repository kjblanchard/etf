
#include <SupergoonEngine/log.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <Supergoon/World/Level.hpp>
namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {};

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
	ContentRegistry::LoadAllContent();
	level->CreateBackgroundImage();
	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Update() {
	if (KeyDown(KeyboardKeys::Key_S)) {
		level->cameraY += 1.3;
	}
	if (KeyDown(KeyboardKeys::Key_D)) {
		level->cameraX += 1.3;
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		level->cameraY -= 1.3;
	}
	if (KeyDown(KeyboardKeys::Key_A)) {
		level->cameraX -= 1.3;
	}
}

void BlackjackGame::Draw() {
	level->Draw();
#ifdef imgui
	Widgets::ShowWidgets();
#endif
}

REGISTER_GAME(BlackjackGame);
