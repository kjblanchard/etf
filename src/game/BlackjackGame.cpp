
#include <SupergoonEngine/log.h>
#include <box2d/box2d.h>

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
namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {};

}
using namespace Supergoon;
Level *level;
Image *character;
RigidbodyComponent *playerRb;

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
};

void BlackjackGame::Start() {
	level = new Level("debugTown");
	character = ContentRegistry::CreateContent<Image>("assets/img/player1.png").get();
	ContentRegistry::LoadAllContent();
	level->CreateBackgroundImage();
	ContentRegistry::LoadAllContent();
	playerRb = new RigidbodyComponent(*level);
}

void BlackjackGame::Update() {
	if (KeyDown(KeyboardKeys::Key_S)) {
		Vector2 force{0, -500};
		playerRb->ApplyForceToBody(force);
	}
	if (KeyDown(KeyboardKeys::Key_D)) {
		Vector2 force{500, 0};
		playerRb->ApplyForceToBody(force);
	}
	if (KeyDown(KeyboardKeys::Key_W)) {
		Vector2 force{0, 500};
		playerRb->ApplyForceToBody(force);
	}
	if (KeyDown(KeyboardKeys::Key_A)) {
		Vector2 force{-500, 0};
		playerRb->ApplyForceToBody(force);
	}
	if (KeyJustReleased(KeyboardKeys::Key_A)) {
		// b2Body_SetLinearVelocity(m_playerId, b2Vec2{0, 0});
	}
	if (KeyDown(KeyboardKeys::Key_K)) {
	}
	if (KeyDown(KeyboardKeys::Key_L)) {
	}
	if (KeyDown(KeyboardKeys::Key_I)) {
	}
	if (KeyDown(KeyboardKeys::Key_J)) {
	}
	level->PhysicsUpdate();
}

void BlackjackGame::Draw() {
	auto graphics = Graphics::Instance();
	auto dstVec = playerRb->Location();
	auto src = RectangleF{0, 0, 27, 31};
	auto dst = RectangleF{(float)(int)dstVec.X, (float)(int)dstVec.Y, 27, 31};
	level->cameraX = dstVec.X;
	level->Draw();
	character->Draw(src, dst);
	auto dstRect = RectangleF{(float)dstVec.X, (float)dstVec.Y, 16, 16};
	graphics.DrawRect(dstRect, Color{255, 0, 0, 255});
#ifdef imgui
	Widgets::ShowWidgets();
#endif
}

REGISTER_GAME(BlackjackGame);
