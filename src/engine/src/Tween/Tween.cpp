#include <SupergoonEngine/easing.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Game.hpp>
#include <Supergoon/Tween/Tween.hpp>

using namespace Supergoon;
Tween::Tween(float start, float end, float duration, float* value, Easings ease) : value(value), _begin(start), _end(end), _duration(duration), _easeType(ease) {
}
Tween::Tween(float start, float end, float duration, int* value, Easings ease) : value(value), _begin(start), _end(end), _duration(duration), _easeType(ease) {
}
Tween::Tween(float waitTime) : value(std::monostate()), _duration(waitTime) {}

void Tween::UpdateInternal() {
	// If this is a monostate, it is a blank tween and doesn't need to update anything
	if (std::holds_alternative<std::monostate>(value)) {
		return;
	}
	float progress = 0;
	float progressVal = 0;
	switch (_easeType) {
		case Easings::Linear:
			progress = geLinearInterpolation(_currentDuration / _duration);
			progressVal = _begin + ((_end - _begin) * progress);
			break;
		default:
			sgLogError("Ease type not implemented!");
			return;
	}
	if (_currentDuration > _duration) {
		progressVal = _end;
	}
	// Use std::visit to handle the different types
	std::visit([&progressVal](auto&& arg) {
		using T = std::decay_t<decltype(arg)>;
		if constexpr (std::is_same_v<T, int*>) {
			*arg = progressVal;
		} else if constexpr (std::is_same_v<T, float*>) {
			*arg = progressVal;
		}
	},
			   value);	// Pass the variant to std::visit
}

bool Tween::Update() {
	if (Complete()) {
		return true;
	}
	if (_duration == 0 && StartFunc) {
		StartFunc();
	}
	_currentDuration += Game::DeltaTime();
	UpdateInternal();
	if (UpdateFunc) {
		UpdateFunc();
	}
	if (Complete() && EndFunc) {
		EndFunc();
	}
	return Complete();
}
