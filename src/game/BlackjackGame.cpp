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

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
};
Level *level = nullptr;

static void playLogos() {
	UI::LoadUIFromFile("logos");
	auto ui = UI::UIInstance;
	auto thing = (ImageObject *)ui->Children["logoImage"].get();
	auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	auto animator = new UIObjectAnimatorBase("Logos");
	auto fadeInTween = new Tween(0, 255, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween = new Tween(255, 0, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	// auto waitTween = new Tween(1.0);
	auto fadeInTween2 = new Tween(0, 255, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween2 = new Tween(255, 0, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	fadeOutTween2->EndFunc = []() {
		level = new Level("debugTown");
		// level->CreateBackgroundImage();
		LoadPlayers();
		LoadAnimationComponents();
		UI::UIInstance->Visible = false;
		UI::UIInstance->Enabled = false;
		ContentRegistry::LoadAllContent();
	};
	animator->AddUIObjectTween(fadeInTween, thing);
	animator->AddUIObjectTween(fadeOutTween, thing);
	// animator->AddUIObjectTween(waitTween, nullptr);
	animator->AddUIObjectTween(fadeInTween2, thing2);
	animator->AddUIObjectTween(fadeOutTween2, thing2);
	UI::Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Start() {
	playLogos();
}

void BlackjackGame::Update() {
	if (level) {
		PlayerInput();
		UpdateAnimationComponents();
		UpdateCamera();
	}
	UI::Update();
}

void BlackjackGame::Draw() {
	if (level) {
		level->Draw();
		DrawAnimationComponents();
		DrawImages();
		DrawDebugBoxes();
		ShowPlayerColliderWindow();
	}
	UI::Draw();
#ifdef imgui
	Widgets::ShowWidgets();
#endif
}

void BlackjackGame::Reset() {
	if (level) {
		delete level;
		level = nullptr;
	}
}

REGISTER_GAME(BlackjackGame);
