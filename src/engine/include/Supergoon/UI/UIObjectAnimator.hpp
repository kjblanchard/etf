#pragma once
#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
#include <type_traits>
#include <variant>
namespace Supergoon {

struct UIObjectAnimatorBase {
	//  For a single Tween with float
	UIObjectAnimatorBase(float start, float end, float duration, float* value);
	//  For a single tween with int
	UIObjectAnimatorBase(float start, float end, float duration, int* value);
	// Create a Animator that handles custom sequences one by one.
	UIObjectAnimatorBase(std::string name);
	void AddUIObjectTween(Tween* tween, UIObject* obj);
	void Play();
	void Update();
	void Stop();
	inline bool IsPlaying() { return _playing; }
	std::string Name;
	std::unique_ptr<Sequence> SequenceToPlay;

   private:
	bool _playing = false;
};

}  // namespace Supergoon
