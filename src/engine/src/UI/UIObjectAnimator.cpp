#include <Supergoon/UI/UIObject.hpp>
#include <Supergoon/UI/UIObjectAnimator.hpp>
using namespace Supergoon;

UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, float* value) {
	SequenceToPlay = std::make_unique<Sequence>();
	SequenceToPlay->Tweens.push_back(std::make_shared<Tween>(start, end, duration, value, Supergoon::Easings::Linear));
}
UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, int* value) {
	SequenceToPlay = std::make_unique<Sequence>();
	SequenceToPlay->Tweens.push_back(std::make_shared<Tween>(start, end, duration, value, Supergoon::Easings::Linear));
}

UIObjectAnimatorBase::UIObjectAnimatorBase(std::string name) : Name(name), SequenceToPlay(std::make_unique<Sequence>()) {
}
void UIObjectAnimatorBase::AddUIObjectTween(Tween* tween, UIObject* obj) {
	if (obj) {
		tween->UpdateFunc = [obj]() { obj->Dirty = true; };
	}
	SequenceToPlay->Tweens.push_back(std::shared_ptr<Tween>(tween));
}

void UIObjectAnimatorBase::Update() {
	if (_playing) {
		SequenceToPlay->Update();
		if (SequenceToPlay->IsComplete()) {
			_playing = false;
		}
	}
}

void UIObjectAnimatorBase::Stop() {
	_playing = false;
	SequenceToPlay->Restart();
}
