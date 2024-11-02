#include <ft2build.h>

#include <Debug/PlayerCollider.hpp>
#include <Entities/PlayerStart.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>
#include FT_FREETYPE_H
namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
	{"Start", [](TiledMap::TiledObject &object) {
		 return NewPlayerSpawn(object);
	 }},
};
}
using namespace Supergoon;
static bool skipLogos = true;
static bool inGame = false;
static void loadLevel() {
	LoadPlayers();
	LoadAnimationComponents();
	UI::UIInstance->Visible = false;
	UI::UIInstance->Enabled = false;
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
	auto thing = (ImageObject *)ui->Children["logoImage"].get();
	auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	auto animator = new UIObjectAnimatorBase("Logos");
	auto fadeInTween = new Tween(0, 255, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween = new Tween(255, 0, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeInTween2 = new Tween(0, 255, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween2 = new Tween(255, 0, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	fadeOutTween2->EndFunc = []() {
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)"debugTown");
	};
	animator->AddUIObjectTween(fadeInTween, thing);
	animator->AddUIObjectTween(fadeOutTween, thing);
	animator->AddUIObjectTween(fadeInTween2, thing2);
	animator->AddUIObjectTween(fadeOutTween2, thing2);
	UI::Animators.push_back(std::shared_ptr<UIObjectAnimatorBase>(animator));

	ContentRegistry::LoadAllContent();
}

void BlackjackGame::Start() {
	Level::LoadFunc = loadLevel;
	FT_Library library;
	auto error = FT_Init_FreeType(&library);
	if (!skipLogos) {
		playLogos();
	} else {
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)"debugTown");
	}
}

void BlackjackGame::Update() {
	if (inGame) {
		PlayerInput();
		UpdateAnimationComponents();
		UpdateCamera();
		if (KeyJustPressed(Supergoon::KeyboardKeys::Key_P)) {
			for (size_t i = 0; i < 30; i++) {
				Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)"debugSouth");
			}
		}
	}
	UI::Update();
}

void BlackjackGame::Draw() {
	if (inGame) {
		Level::Draw();
		DrawAnimationComponents();
		DrawImages();
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
#endif
}

void BlackjackGame::Reset() {
	inGame = false;
	Level::Reset();
}

REGISTER_GAME(BlackjackGame);
