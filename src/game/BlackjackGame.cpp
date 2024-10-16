
#include <SupergoonEngine/log.h>
#include <box2d/box2d.h>

#include <Entities/PlayerStart.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Input.hpp>
#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <Supergoon/World/Level.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/LocationSystem.hpp>
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
Image *character;

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
	level->PhysicsUpdate();
	UpdateLocationsForRbs();
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
#endif
}

REGISTER_GAME(BlackjackGame);
