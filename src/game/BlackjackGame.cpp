
#include <SDL3/SDL.h>

#include <Debug/PlayerCollider.hpp>
#include <Entities/PlayerExit.hpp>
#include <Entities/PlayerStart.hpp>
#include <Entities/TextInteraction.hpp>
#include <Supergoon/Supergoon.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>
#include <Systems/TextInteractionSystem.hpp>
#include <Utilities/Utilities.hpp>

using json = nlohmann::json;
extern json configData;
namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
	{"Start", [](TiledMap::TiledObject &object) {
		 return NewPlayerSpawn(object);
	 }},
	{"Exit", [](TiledMap::TiledObject &object) {
		 return NewPlayerExit(object);
	 }},
	{"TextInteract", [](TiledMap::TiledObject &object) {
		 return NewTextInteraction(object);
	 }}};
}
using namespace Supergoon;
static bool inGame = false;
static void loadLevel() {
	LoadPlayers();
	LoadAnimationComponents();
	StartPlayers();
	// Check if we should show the text at top
	auto display = Level::GetCurrentLevelProperty<std::string>("display");
	auto ui = UI::UIInstance;
	auto textPanel = std::dynamic_pointer_cast<Panel>(UI::UIInstance->Children["textTestingscreen"]);
	assert(textPanel);
	if (display) {
		auto textBox = (UIText *)textPanel->Children["textmanscreen"].get();
		assert(textBox);
		textPanel->SetVisible(true);
		textPanel->SetAlpha(255);
		textBox->UpdateText(*display);
		// TODO this should be different.
		for (auto &&animator : textPanel->Animators) {
			animator->Restart();
			animator->Play();
		}
	} else {
		textPanel->SetVisible(false);
	}
	ContentRegistry::LoadAllContent();
	inGame = true;
}

class BlackjackGame : public Game {
   public:
	void Start() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
};

static void playLogos() {
	UI::LoadUIFromFile("logos");
	auto ui = UI::UIInstance;
	auto thing = (UIImage *)ui->Children["logoImage"].get();
	auto thing2 = (UIImage *)ui->Children["logoImage2"].get();
	auto animator = new UIObjectAnimatorBase("logo");
	auto animator2 = new UIObjectAnimatorBase("logo2");
	auto fadeInTween = new Tween(0, 255, 3.0, thing->AlphaHandle(), Supergoon::Easings::Linear);
	auto fadeOutTween = new Tween(255, 0, 3.0, thing->AlphaHandle(), Supergoon::Easings::Linear);
	auto fadeInTween2 = new Tween(0, 255, 3.0, thing2->AlphaHandle(), Supergoon::Easings::Linear);
	auto fadeOutTween2 = new Tween(255, 0, 3.0, thing2->AlphaHandle(), Supergoon::Easings::Linear);
	fadeOutTween->EndFunc = [animator2]() {
		animator2->Play();
	};
	fadeOutTween2->EndFunc = []() {
		CreateUITextbox("screen", Point{145, 15}, Point{200, 48}, true);
		InitializeTextInteractionUI();
		// setupUINameChangeBox();
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup("debugTown"));
		Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"logoImage");
		Events::PushEvent(Events::BuiltinEvents.UiDestroyObject, 0, (void *)"logoImage2");

		// ui->Children.erase("logoImage");
		// ui->Children.erase("logoImage2");
	};
	animator->AddUIObjectTween(fadeInTween, thing);
	animator->AddUIObjectTween(fadeOutTween, thing);
	animator2->AddUIObjectTween(fadeInTween2, thing2);
	animator2->AddUIObjectTween(fadeOutTween2, thing2);
	thing->Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));
	thing2->Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator2));
	animator->Play();
	// UI::Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Start() {
	Level::LoadFunc = loadLevel;
	auto muted = configData["mute"];
	if (muted) {
		Sound::Instance()->SetGlobalBgmVolume(0);
		Sound::Instance()->SetGlobalSfxVolume(0);
	}
	auto skipLogos = configData["skipLogos"];
	if (!skipLogos) {
		playLogos();
	} else {
		CreateUITextbox("screen", Point{145, 15}, Point{200, 48}, true);
		InitializeTextInteractionUI();
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup("debugTown"));
	}
}

void BlackjackGame::Update() {
	if (inGame) {
		PlayerInput();
		UpdateAnimationComponents();
		UpdateTextInteractions();
		UpdateCamera();
	}
	UI::Update();
}

void BlackjackGame::Draw() {
	if (inGame) {
		Level::Draw();
		DrawAnimationComponents();
		DrawImages();
		DrawTextInteractionDisplay();
	}
	UI::Draw();
#ifdef imgui
	if (GlobalWidget::ShowDebugBoxSolid) {
		DrawDebugBoxesSolid();
	}

	Widgets::ShowWidgets(this);
	PlayerWidget::ShowPlayerColliderWindow();
	if (PlayerWidget::ShowPlayerColliderDebugBox) {
		DrawDebugBoxesPlayer();
	}
	if (PlayerWidget::ShowPlayerExitDebugBox) {
		DrawDebugBoxesPlayerExit();
	}
	if (PlayerWidget::ShowPlayerInteractionDebugBox) {
		DrawDebugBoxesPlayerInteractionBox();
	}
	if (PlayerWidget::ShowInteractionDebugBox) {
		DrawDebugBoxesTextInteractionBox();
	}
#endif
}

void BlackjackGame::Reset() {
	inGame = false;
	Level::Reset();
}

REGISTER_GAME(BlackjackGame);
