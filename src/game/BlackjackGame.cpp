#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <iostream>
using namespace Supergoon;
static int ticks = 0;
class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update(double deltaTime) override;
	void Draw() override;
};

void BlackjackGame::Start() {
	Sound().LoadBgm("town2");
	// Sound().PlayBgm();
}
void BlackjackGame::Update(double) {
	++ticks;
	if (ticks % 60 == 0) {
		Sound().PlaySfx("transition", 1.0f);
	}
}

void BlackjackGame::Draw() {
}

auto gamer = BlackjackGame();