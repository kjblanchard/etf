#include <Supergoon/UI/UIObjectAnimator.hpp>
using namespace Supergoon;

UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, float* value, UIObject* obj)
	: object(obj) {
	SequenceToPlay = std::make_unique<Sequence>();
	SequenceToPlay->Tweens.push_back(std::make_shared<Tween>(start, end, duration, value, Supergoon::Easings::Linear));
}
UIObjectAnimatorBase::UIObjectAnimatorBase(float start, float end, float duration, int* value, UIObject* obj)
	: object(obj) {
	SequenceToPlay = std::make_unique<Sequence>();
	SequenceToPlay->Tweens.push_back(std::make_shared<Tween>(start, end, duration, value, Supergoon::Easings::Linear));
}

void UIObjectAnimatorBase::Play() {
	SequenceToPlay->Restart();
}
void UIObjectAnimatorBase::Update() {
	SequenceToPlay->Update();
	object->Dirty = true;
}
void UIObjectAnimatorBase::Stop() {
	_playing = false;
}
