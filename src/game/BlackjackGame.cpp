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
Sequence *sequence;
UIObjectAnimatorBase *animator;

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
};

void BlackjackGame::Start() {
	ui = UI::LoadUIFromFile("logos");
	auto thing = (ImageObject *)ui->Children["logoImage"].get();
	auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	animator = new UIObjectAnimatorBase("Logos");
	auto fadeTween = new Tween(255, 0, 2.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeTween2 = new Tween(255, 0, 2.0, &thing2->Transparency, Supergoon::Easings::Linear);
	animator->AddUIObjectTween(fadeTween, thing);
	animator->AddUIObjectTween(fadeTween2, thing2);

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
	animator->Update();
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
