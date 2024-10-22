#pragma once
#include <Supergoon/Tween/Tween.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <string>
#include <type_traits>
namespace Supergoon {

struct UIObjectAnimatorBase {
	void Play();
	void Update();
	void Stop();
	float Percentage();
	std::string Name;
	Tween* tween;
	float start;
	float end;
	float* value;
	UIObject* object;
};

// template <typename T, typename = std::enable_if_t<std::is_base_of_v<UIObject, T>>>
// class UIObjectAnimator : UIObjectAnimatorBase {
// 	T* AnimatedUIObject;
// };
}  // namespace Supergoon
