#include <Supergoon/UI/UIObjectAnimator.hpp>
using namespace Supergoon;

// static void updateInternal(const std::variant<float*, int*>& value, Tween* tween) {
// 	// Use std::visit to handle the different types
// 	std::visit([&tween](auto&& arg) {
// 		using T = std::decay_t<decltype(arg)>;
// 		if constexpr (std::is_same_v<T, int*>) {
// 			*arg = tween->Value();
// 		} else if constexpr (std::is_same_v<T, float*>) {
// 			*arg = tween->Value();
// 		}
// 	},
// 			   value);	// Pass the variant to std::visit
// }

UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, float* value, UIObject* obj)
	: tween(std::make_unique<Tween>(start, end, duration, value, Supergoon::Easings::Linear)), value(value), object(obj) {}
UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, int* value, UIObject* obj)
	: tween(std::make_unique<Tween>(start, end, duration, value, Supergoon::Easings::Linear)), value(value), object(obj) {}

void UIObjectAnimatorBase::Play() {
	tween->Restart();
	_playing = true;
}
void UIObjectAnimatorBase::Update() {
	if (!_playing || tween->Complete()) {
		return;
	}
	tween->Update();
	// updateInternal(value, tween.get());
	object->Dirty = true;
}
void UIObjectAnimatorBase::Stop() {
	_playing = false;
}
