#include <Supergoon/Game.hpp>
#include <iostream>
using namespace Supergoon;
class BlackjackGame : public Game {
   public:
	void Start();
	void Update(double deltaTime);
	void Draw();
};

void BlackjackGame::Start() {
	puts("Start");
}
void BlackjackGame::Update(double) {
	puts("Update");
}

void BlackjackGame::Draw() {
	puts("Draw");
}