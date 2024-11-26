#include <SDL3/SDL.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/UI/UIImage.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <fstream>
using namespace Supergoon;
using json = nlohmann::json;
std::unique_ptr<Panel> UI::UIInstance = nullptr;
UIObjectAnimatorBase* UI::_fadeInAnimator = nullptr;
UIObjectAnimatorBase* UI::_fadeOutAnimator = nullptr;
// std::vector<std::shared_ptr<UIObjectAnimatorBase>> UI::Animators;
void UI::RegisterUIEvents() {
	Events::RegisterEventHandler(Events::BuiltinEvents.UiDestroyObject, [](int, void* name, void*) {
		auto ui = UI::UIInstance.get();
		auto nameString = (const char*)name;
		assert(nameString);
		ui->Children.erase(nameString);
	});
}

Panel* UI::Initialize() {
	if (UIInstance) {
		UIInstance.reset();
	}
	UIInstance = std::make_unique<Panel>();
	auto graphics = Graphics::Instance();
	UIInstance->Bounds = RectangleF{0, 0, (float)graphics->LogicalWidth(), (float)graphics->LogicalHeight()};
	auto fadeImage = new UIImage(UIInstance.get(), "Fade Panel");
	fadeImage->SetAlpha(0);
	auto path = std::string(SDL_GetBasePath()) + "assets/img/null.png";
	fadeImage->ImagePtr = ContentRegistry::CreateContent<Image>(path);
	fadeImage->ImagePtr->SetImageColor({0, 0, 0, 255});
	fadeImage->SetVisible(true);
	fadeImage->Bounds = RectangleF{0, 0, (float)graphics->LogicalWidth(), (float)graphics->LogicalHeight()};
	fadeImage->Offset.X = 0;
	fadeImage->Offset.Y = 0;
	auto fadeInAnimator = std::make_shared<UIObjectAnimatorBase>("fadein");
	auto fadeOutAnimator = std::make_shared<UIObjectAnimatorBase>("fadeout");
	auto fadeOutTween = new Tween(0, 255, 0.3, fadeImage->AlphaHandle(), Supergoon::Easings::Linear);
	auto fadeInTween = new Tween(255, 0, 0.3, fadeImage->AlphaHandle(), Supergoon::Easings::Linear);
	fadeInAnimator->AddUIObjectTween(fadeInTween, fadeImage);
	fadeOutAnimator->AddUIObjectTween(fadeOutTween, fadeImage);
	fadeImage->Animators.push_back(fadeInAnimator);
	fadeImage->Animators.push_back(fadeOutAnimator);
	_fadeOutAnimator = fadeOutAnimator.get();
	_fadeInAnimator = fadeInAnimator.get();
	// rootPanel->Children[name] = fadePanel;
	return UIInstance.get();
}

void UI::Update() {
	if (UIInstance) {
		UIInstance->UpdateInternal();
	}
}

void UI::Draw() {
	if (UIInstance) {
		UIInstance->Draw();
	}
}

void UI::LoadUIFromFile(std::string filename, Panel* parentPanel) {
	std::string jsonPath = std::string(SDL_GetBasePath()) + "assets/ui/" + filename + ".json";
	std::ifstream fileStream(jsonPath);
	auto j = json::parse(fileStream);
	for (auto& jsonChild : j) {
		if (jsonChild["type"] == "Image") {
			auto name = jsonChild["name"].get<std::string>();
			auto child = std::make_shared<UIImage>(parentPanel, jsonChild);
			parentPanel->Children[name] = child;
		}
	}
}
void UI::SetFadeOutEndFunc(std::function<void()> func) {
	auto lastTween = _fadeOutAnimator->SequenceToPlay->Tweens.back();
	if (lastTween) {
		lastTween->EndFunc = func;
	}
}
void UI::SetFadeInStartFunc(std::function<void()> func) {
	auto lastTween = _fadeInAnimator->SequenceToPlay->Tweens.back();
	if (lastTween) {
		lastTween->StartFunc = func;
	}
}
