#pragma once
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
#include <Supergoon/World/Level.hpp>
#include <memory>
#include <string>
#include <vector>
namespace Supergoon {

class Panel;
class UI {
   public:
	static Panel* Initialize();
	static void LoadUIFromFile(std::string filename, Panel* rootPanel = UIInstance);
	static void Draw();
	static void Update();
	static void Reset();
	static void RegisterUIEvents();
	static Panel* UIInstance;

	// Set when the UI

	// Functions to handle the global Fade Panel.
   public:
	inline static void FadeIn() { _fadeInAnimator->Restart(); }
	inline static void FadeOut() { _fadeOutAnimator->Restart(); }
	inline static bool FadingOut() { return _fadeOutAnimator->IsPlaying(); }
	inline static bool FadingIn() { return _fadeInAnimator->IsPlaying(); }
	static void SetFadeOutEndFunc(std::function<void()> func);
	static void SetFadeInStartFunc(std::function<void()> func);

   private:
	static UIObjectAnimatorBase* _fadeInAnimator;
	static UIObjectAnimatorBase* _fadeOutAnimator;
};
}  // namespace Supergoon
