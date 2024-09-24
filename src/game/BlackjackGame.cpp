#include <Supergoon/Bgm.hpp>
#include <Supergoon/Game.hpp>
#include <iostream>
using namespace Supergoon;
class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update(double deltaTime) override;
	void Draw() override;
};

void BlackjackGame::Start() {
	Sound().LoadBgm("town2");
	Sound().PlayBgm();
}
void BlackjackGame::Update(double) {
}

void BlackjackGame::Draw() {
}

auto gamer = BlackjackGame();