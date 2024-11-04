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
	static std::vector<std::shared_ptr<UIObjectAnimatorBase>> Animators;
	static Panel* UIInstance;
	inline static void FadeIn() { _fadeInAnimator->Play(); }
	inline static void FadeOut() { _fadeOutAnimator->Play(); }

   private:
	static UIObjectAnimatorBase* _fadeInAnimator;
	static UIObjectAnimatorBase* _fadeOutAnimator;
};
}  // namespace Supergoon
