
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#include <iostream>
using namespace Supergoon;
// static int ticks = 0;
// static Sfx* _sfx;
class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update(double deltaTime) override;
	void Draw() override;
};

void BlackjackGame::Start() {
}
void BlackjackGame::Update(double) {
}

void BlackjackGame::Draw() {
	Widgets::ShowWidgets();
}

auto gamer = BlackjackGame();
