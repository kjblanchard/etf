#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
using namespace Supergoon;

void Sequence::Update() {
	auto complete = true;
	for (auto &&tween : Tweens) {
		if (tween->Update()) {
			continue;
		}
		complete = false;
		break;
	}
	_complete = complete;
}
int Sequence::Percent() {
	auto currentLength = 0.0f;
	auto totalLength = 0.0f;
	for (auto &&tween : Tweens) {
		currentLength += tween->CurrentLength();
		totalLength += tween->TotalLength();
	}
	return (currentLength / totalLength) * 100;
}

void Sequence::Restart() {
	for (auto &&tween : Tweens) {
		tween->Restart();
	}
	_complete = false;
}
