#include <Supergoon/UI/UIObjectAnimator.hpp>
using namespace Supergoon;
void UIObjectAnimatorBase::Play() {
	*value = start;
}
void UIObjectAnimatorBase::Update() {
	if (tween->Complete()) {
		return;
	}
	tween->Update();
	object->Dirty = true;
}
void UIObjectAnimatorBase::Stop() {
}
float UIObjectAnimatorBase::Percentage() {
	return *value / end;
}
