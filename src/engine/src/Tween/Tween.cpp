#include <SupergoonEngine/easing.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Game.hpp>
#include <Supergoon/Tween/Tween.hpp>

using namespace Supergoon;
Tween::Tween(float start, float end, float duration, float* value, Easings ease) : _begin(start), _end(end), _duration(duration), value(value), _easeType(ease) {
}
Tween::Tween(float start, float end, float duration, int* value, Easings ease) : _begin(start), _end(end), _duration(duration), _easeType(ease) {
}

void Tween::UpdateInternal() {
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
	if (_currentDuration >= _duration)
		return true;
	_currentDuration += Game::DeltaTime();
	UpdateInternal();
	return _currentDuration >= _duration;
}
