#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
using namespace Supergoon;

void Sequence::Update() {
	auto complete = true;
	for (auto &&tween : Tweens) {
		if (tween->Complete()) {
			continue;
		}
		tween->Update();
		complete = false;
		break;
	}
	_complete = complete;
}
void Sequence::Restart() {
	for (auto &&tween : Tweens) {
		tween->Restart();
	}
	_complete = false;
}
