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
};
Level *level = nullptr;

void BlackjackGame::Start() {
	auto ui = UI::Initialize();
	UI::LoadUIFromFile("logos", ui);
	auto thing = (ImageObject *)ui->Children["logoImage"].get();
	auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	thing2->Visible = true;
	auto animator = new UIObjectAnimatorBase("Logos");
	auto fadeTween = new Tween(255, 0, 2.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto waitTween = new Tween(5.0);
	auto fadeTween2 = new Tween(255, 0, 2.0, &thing2->Transparency, Supergoon::Easings::Linear);
	fadeTween2->EndFunc = []() {
		level = new Level("debugTown");
		level->CreateBackgroundImage();
		LoadPlayers();
		LoadAnimationComponents();
		UI::UIInstance->Visible = false;
		UI::UIInstance->Enabled = false;
		ContentRegistry::LoadAllContent();
	};
	animator->AddUIObjectTween(fadeTween, thing);
	animator->AddUIObjectTween(waitTween, nullptr);
	animator->AddUIObjectTween(fadeTween2, thing2);
	UI::Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));

	ContentRegistry::LoadAllContent();
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

REGISTER_GAME(BlackjackGame);
