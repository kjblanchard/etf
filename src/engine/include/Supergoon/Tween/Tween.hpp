#pragma once
namespace Supergoon {
enum class Easings {
	Linear,
	QuadEaseIn,
	QuadEaseOut,
	QuadEaseInOut,
	CubicEaseIn,
	CubicEaseOut,
	CubicEaseInOut,
	QuarticEaseIn,
	QuarticEaseOut,
	QuarticEaseInOut,
	QuinticEaseIn,
	QuinticEaseOut,
	QuinticEaseInOut,
	SineEaseIn,
	SineEaseOut,
	SineEaseInOut,
	CircularEaseIn,
	CircularEaseOut,
	CircularEaseInOut,
	ExponentialEaseIn,
	ExponentialEaseOut,
	ExponentialEaseInOut,
	ElasticEaseIn,
	ElasticEaseOut,
	ElasticEaseInOut,
	BackEaseIn,
	BackEaseOut,
	BackEaseInOut,
	BounceEaseIn,
	BounceEaseOut,
	BounceEaseInOut,
};

class Tween {
   public:
	// Start value, end value,
	Tween(float start, float end, float duration, Easings ease);
	float Value();
	bool Update();
	inline void Restart() { _currentDuration = 0; }
	inline bool Complete() { return _currentDuration >= _duration; }

   private:
	float _begin = 0, _end = 0, _currentDuration = 0, _duration = 0;
	Easings _easeType;
};

}  // namespace Supergoon
