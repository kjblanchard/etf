
#include <Components/BattleComponent.hpp>
#include <Debug/BattleWidget.hpp>
#include <Debug/PlayerCollider.hpp>
#include <Entities/Battle/BattleLocation.hpp>
#include <Entities/Battle/BattleZone.hpp>
#include <Entities/PlayerExit.hpp>
#include <Entities/PlayerStart.hpp>
#include <Entities/TextInteraction.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIImage.hpp>
#include <Supergoon/UI/UIText.hpp>
#include <Supergoon/pch.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/Battle/BattleLocationSystem.hpp>
#include <Systems/Battle/BattleSystem.hpp>
#include <Systems/Battle/BattleUISystem.hpp>
#include <Systems/Battle/BattleZoneSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/GameStateSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>
#include <Systems/TextInteractionSystem.hpp>
#include <Utilities/Events.hpp>
#include <Utilities/Utilities.hpp>
#ifdef imgui
#include <Supergoon/Widgets/Global.hpp>
#include <Supergoon/Widgets/Widgets.hpp>
#endif

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
	{"BattleZone", [](TiledMap::TiledObject &object) {
		 return NewBattleZone(object);
	 }},
	{"BattleLocation", [](TiledMap::TiledObject &object) {
		 return NewBattleLocation(object);
	 }},
	{"TextInteract", [](TiledMap::TiledObject &object) {
		 return NewTextInteraction(object);
	 }}};
}
using namespace Supergoon;
static bool inGame = false;
static void loadLevel() {
	LoadPlayers();
	LoadBattlers();
	LoadAnimationComponents();
	LoadTextInteractions();
	StartPlayers();
	StartBattlers();
	// Check if we should show the text at top
	auto display = Level::GetCurrentLevelProperty<std::string>("display");
	// auto ui = UI::UIInstance.get();
	auto textPanel = dynamic_cast<Panel *>(UI::UIInstance->GetChildByName("textTestingscreen"));
	assert(textPanel);
	if (display) {
		auto textBox = (UIText *)textPanel->GetChildByName("textmanscreen");
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
	auto gamestateGameObject = GameObject::GetGameObjectWithComponents<GameState>();
	assert(gamestateGameObject);
	auto battleComponent = gamestateGameObject->HasComponent<BattleComponent>();
	if (!battleComponent) {
		auto battleComp = BattleComponent();
		battleComp.BattleId = 0;
		battleComp.BattleMapId = 0;
		gamestateGameObject->AddComponent<BattleComponent>(battleComp);
	}
	ContentRegistry::LoadAllContent();
	inGame = true;
}

class BlackjackGame : public Game {
   public:
	~BlackjackGame() = default;
	void Start() override;
	void Update() override;
	void Draw() override;
	void Reset() override;
};

static void playLogos() {
	UI::LoadUIFromFile("logos");
	auto ui = UI::UIInstance.get();
	auto thing = (UIImage *)ui->GetChildByName("logoImage");
	auto thing2 = (UIImage *)ui->GetChildByName("logoImage2");
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
	InitializeEvents();
	InitializeCamera();
	InitializeBattleUI();
}

void BlackjackGame::Update() {
	if (inGame) {
		UpdateGameState();
		PlayerInput();
		UpdateBattleZones();
		UpdateAnimationComponents();
		UpdateTextInteractions();
		UpdateBattle();
		UpdateBattleUI();
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
	BattleWidget::ShowBattleWidget();
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
