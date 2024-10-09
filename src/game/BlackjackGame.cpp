
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
using namespace Supergoon;

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
};

void BlackjackGame::Start() {
}

void BlackjackGame::Update() {
}

void BlackjackGame::Draw() {
	Widgets::ShowWidgets();
}

REGISTER_GAME(BlackjackGame);
