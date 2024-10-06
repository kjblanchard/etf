#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <iostream>
using namespace Supergoon;
static int ticks = 0;
static Sfx* _sfx;
class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update(double deltaTime) override;
	void Draw() override;
};

void BlackjackGame::Start() {
	Sound().LoadBgm("town2");
	_sfx = ContentRegistry::CreateContent<Sfx>("transition").get();
	_sfx->Load();
}
void BlackjackGame::Update(double) {
	++ticks;
	if (ticks % 60 == 0) {
		Sound().PlaySfx(_sfx, 1.0f);
	}
}

void BlackjackGame::Draw() {
}

auto gamer = BlackjackGame();
