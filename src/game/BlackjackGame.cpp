
#include <SDL3/SDL.h>

#include <Debug/PlayerCollider.hpp>
#include <Entities/PlayerExit.hpp>
#include <Entities/PlayerStart.hpp>
#include <Supergoon/Supergoon.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <Systems/AsepriteSystem.hpp>
#include <Systems/CameraSystem.hpp>
#include <Systems/DebugDrawSystem.hpp>
#include <Systems/ImageSystem.hpp>
#include <Systems/PlayerSystem.hpp>

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
};
}
using namespace Supergoon;
static bool inGame = false;
static void loadLevel() {
	LoadPlayers();
	LoadAnimationComponents();
	StartPlayers();
	// Check if we should show the text at top
	auto display = Level::GetCurrentLevelProperty<std::string>("display");
	auto textPanel = std::dynamic_pointer_cast<Panel>(UI::UIInstance->Children["textTesting"]);
	assert(textPanel);
	if (display) {
		auto textBox = (UIText *)textPanel->Children["textman"].get();
		assert(textBox);
		textPanel->Visible = true;
		textBox->UpdateText(*display);
		// do something
	} else {
		textPanel->Visible = false;
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
// TODO this shouldn't be here
static void setupUINameChangeBox() {
	auto ui = UI::UIInstance;
	auto textPanel = std::make_shared<Panel>(ui);
	textPanel->Offset = {15, 8};
	auto text = std::make_shared<UIText>(textPanel.get(), "Hello world!");
	text->Offset = {12, 21};
	textPanel->Children["textman"] = text;
	ui->Children["textTesting"] = textPanel;
	// Test creating the uitextbox
	// First, lets load in the picture for uiimage so that we can draw from it to the new one
	auto path = std::string(SDL_GetBasePath()) + "assets/img/uibase.png";
	auto uiImageFull = ContentRegistry::CreateContent<Image>(path);
	uiImageFull->LoadContent();
	// Create ui text image of the right size as a render target
	float fullSizeX = 128;
	float fullSizeY = 64;
	auto textBoxImage = ContentRegistry::CreateContent<Image, int, int>("uitextbox", (int)fullSizeX, (int)fullSizeY);
	textBoxImage->LoadContent();
	// Set the background
	textBoxImage->Clear({80, 0, 80, 220});
	float sizeX = 8;
	float sizeY = 9;
	textBoxImage->SetAlpha(200);
	// Draw the corners
	// tl
	auto srcRect = RectangleF{0, 0, sizeX, sizeY};
	auto dstRect = RectangleF{0, 0, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// tr
	srcRect = RectangleF{uiImageFull->Width() - sizeX, 0, sizeX, sizeY};
	dstRect = RectangleF{fullSizeX - sizeX, 0, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// bl
	srcRect = RectangleF{0, uiImageFull->Height() - sizeY, sizeX, sizeY};
	dstRect = RectangleF{0, fullSizeY - sizeY, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// br
	srcRect = RectangleF{uiImageFull->Width() - sizeX, uiImageFull->Height() - sizeY, sizeX, sizeY};
	dstRect = RectangleF{fullSizeX - sizeX, fullSizeY - sizeY, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// draw the bars
	int length = fullSizeX - (sizeX);
	int height = fullSizeY - (sizeY);
	// top
	srcRect = RectangleF{1 + sizeX, 0, 1, sizeY};
	for (size_t i = sizeX; i < length; i++) {
		dstRect = RectangleF{(float)i, 0, 1, sizeY};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// bottom
	for (size_t i = sizeX; i < length; i++) {
		dstRect = RectangleF{(float)i, fullSizeY - sizeY + 4, 1, sizeY};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// left
	srcRect = RectangleF({0, sizeY + 1, sizeX, 1});
	for (size_t i = sizeY; i < height; i++) {
		dstRect = RectangleF{0, (float)i, sizeX, 1};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// right
	for (size_t i = sizeY; i < height; i++) {
		dstRect = RectangleF{fullSizeX - sizeX + 3, (float)i, sizeX, 1};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}

	// Add this image to the ui panel
	auto textBoxUIImage = std::make_shared<ImageObject>(textPanel.get());
	textBoxUIImage->ImagePtr = textBoxImage;
	textBoxUIImage->Visible = true;
	textBoxUIImage->Bounds = RectangleF{0, 0, (float)textBoxImage->Width(), (float)textBoxImage->Height()};
	textBoxUIImage->Offset.X = 0;
	textBoxUIImage->Offset.Y = 0;
	textPanel->Children["uitextbox"] = textBoxUIImage;
	textPanel->Visible = true;
}

static void playLogos() {
	UI::LoadUIFromFile("logos");
	auto ui = UI::UIInstance;
	auto thing = (ImageObject *)ui->Children["logoImage"].get();
	auto thing2 = (ImageObject *)ui->Children["logoImage2"].get();
	auto animator = new UIObjectAnimatorBase("logo");
	auto animator2 = new UIObjectAnimatorBase("logo2");
	auto fadeInTween = new Tween(0, 255, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween = new Tween(255, 0, 3.0, &thing->Transparency, Supergoon::Easings::Linear);
	auto fadeInTween2 = new Tween(0, 255, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	auto fadeOutTween2 = new Tween(255, 0, 3.0, &thing2->Transparency, Supergoon::Easings::Linear);
	fadeOutTween->EndFunc = [animator2]() {
		animator2->Play();
	};
	fadeOutTween2->EndFunc = []() {
		setupUINameChangeBox();
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
		// UI::LoadUIFromFile("logos");
		setupUINameChangeBox();
		Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup("debugTown"));
	}
}

void BlackjackGame::Update() {
	if (inGame) {
		PlayerInput();
		UpdateAnimationComponents();
		UpdateCamera();
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
	if (PlayerWidget::ShowPlayerExitDebugBox) {
		DrawDebugBoxesPlayerExit();
	}
#endif
}

void BlackjackGame::Reset() {
	inGame = false;
	Level::Reset();
}

REGISTER_GAME(BlackjackGame);
