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
Tween *fadeTween;
Tween *fadeTween2;
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
	// auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	animator = new UIObjectAnimatorBase(255, 0, 3.0, &thing->Transparency, thing);
	// animator->value = &thing->Transparency;
	// animator->object = thing;
	// fadeTween = new Tween();
	// fadeTween2 = new Tween();
	// sequence = new Sequence();
	// sequence->Tweens.push_back(std::make_shared<Tween>(255, 0, 5.0, &thing->Transparency, Supergoon::Easings::Linear));
	// sequence->Tweens.push_back(std::make_shared<Tween>(255, 0, 5.0, &thing2->Transparency, Supergoon::Easings::Linear));
	// animator->tween = fadeTween;
	// animator->Play();

	// level = new Level("debugTown");
	// level->CreateBackgroundImage();
	// LoadPlayers();
	// LoadAnimationComponents();

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Update() {
	// auto thing = (ImageObject *)ui->Children["logoImage"].get();
	// auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	// PlayerInput();
	// UpdateAnimationComponents();
	// UpdateCamera();
	// animator->Update();
	// fadeTween->Update();
	sequence->Update();
	ui->UpdateInternal();
	// auto thing = (ImageObject *)ui->Children["logoImage"].get();
	// if (!thing) {
	// 	return;
	// }
	// thing->Transparency = fadeTween->Value();
	// thing->Dirty = true;
	// thing->Dirty = true;
	// thing2->Dirty = true;
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
