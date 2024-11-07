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
Panel* UI::UIInstance = nullptr;
UIObjectAnimatorBase* UI::_fadeInAnimator = nullptr;
UIObjectAnimatorBase* UI::_fadeOutAnimator = nullptr;
// std::vector<std::shared_ptr<UIObjectAnimatorBase>> UI::Animators;
void UI::RegisterUIEvents() {
	Events::RegisterEventHandler(Events::BuiltinEvents.UiDestroyObject, [](int, void* name, void*) {
		auto ui = UI::UIInstance;
		auto nameString = (const char*)name;
		assert(nameString);
		ui->Children.erase(nameString);
	});
}

Panel* UI::Initialize() {
	if (UIInstance) {
		return UIInstance;
	}
	auto rootPanel = new Panel();
	auto graphics = Graphics::Instance();
	rootPanel->Bounds = RectangleF{0, 0, (float)graphics->LogicalWidth(), (float)graphics->LogicalHeight()};
	UIInstance = rootPanel;
	auto name = "fadePanel";
	auto fadePanel = std::make_shared<ImageObject>(rootPanel);
	fadePanel->SetAlpha(0);
	auto path = std::string(SDL_GetBasePath()) + "assets/img/null.png";
	fadePanel->ImagePtr = ContentRegistry::CreateContent<Image>(path);
	fadePanel->ImagePtr->SetImageColor({0, 0, 0, 255});
	fadePanel->Visible = true;
	fadePanel->Bounds = RectangleF{0, 0, (float)graphics->LogicalWidth(), (float)graphics->LogicalHeight()};
	fadePanel->Offset.X = 0;
	fadePanel->Offset.Y = 0;
	auto fadeInAnimator = std::make_shared<UIObjectAnimatorBase>("fadein");
	auto fadeOutAnimator = std::make_shared<UIObjectAnimatorBase>("fadeout");
	auto fadeOutTween = new Tween(0, 255, 0.3, fadePanel->AlphaHandle(), Supergoon::Easings::Linear);
	auto fadeInTween = new Tween(255, 0, 0.3, fadePanel->AlphaHandle(), Supergoon::Easings::Linear);
	fadeInAnimator->AddUIObjectTween(fadeInTween, fadePanel.get());
	fadeOutAnimator->AddUIObjectTween(fadeOutTween, fadePanel.get());
	fadePanel->Animators.push_back(fadeInAnimator);
	fadePanel->Animators.push_back(fadeOutAnimator);
	_fadeOutAnimator = fadeOutAnimator.get();
	_fadeInAnimator = fadeInAnimator.get();
	rootPanel->Children[name] = fadePanel;
	return rootPanel;
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
void UI::Reset() {
	if (UIInstance) {
		delete UIInstance;
		UIInstance = nullptr;
	}
}

void UI::LoadUIFromFile(std::string filename, Panel* parentPanel) {
	std::string jsonPath = std::string(SDL_GetBasePath()) + "assets/ui/" + filename + ".json";
	std::ifstream fileStream(jsonPath);
	auto j = json::parse(fileStream);
	for (auto& jsonChild : j) {
		if (jsonChild["type"] == "Image") {
			auto name = jsonChild["name"].get<std::string>();
			auto child = std::make_shared<ImageObject>(parentPanel, jsonChild);
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
