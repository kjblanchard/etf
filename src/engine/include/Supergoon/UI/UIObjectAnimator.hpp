#pragma once
#include <Supergoon/Tween/Tween.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
namespace Supergoon {

struct UIObjectAnimatorBase {
	//  For a single Tween with float
	UIObjectAnimatorBase(float start, float end, float duration, float* value, UIObject* obj);
	//  For a single tween with int
	UIObjectAnimatorBase(float start, float end, float duration, int* value, UIObject* obj);
	void Play();
	void Update();
	void Stop();
	std::string Name;
	std::unique_ptr<Tween> tween;
	std::variant<float*, int*> value;
	UIObject* object;

   private:
	bool _playing;
};

// template <typename T, typename = std::enable_if_t<std::is_base_of_v<UIObject, T>>>
// class UIObjectAnimator : UIObjectAnimatorBase {
// 	T* AnimatedUIObject;
// };
}  // namespace Supergoon
