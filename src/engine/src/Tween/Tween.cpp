#include <SupergoonEngine/easing.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Game.hpp>
#include <Supergoon/Tween/Tween.hpp>

using namespace Supergoon;
Tween::Tween(float start, float end, float duration, Easings ease) : _begin(start), _end(end), _duration(duration), _easeType(ease) {
}
float Tween::Value() {
	if (_currentDuration >= _duration) {
		return _end;
	}
	float progress = 0;
	switch (_easeType) {
		case Easings::Linear:
			progress = geLinearInterpolation(_currentDuration / _duration);
			return _begin + ((_end - _begin) * progress);
		default:
			sgLogError("Ease type not implemented!");
			return 0;
	}
}
bool Tween::Update() {
	if (_currentDuration >= _duration)
		return true;
	_currentDuration += Game::DeltaTime();
	return _currentDuration >= _duration;
}
