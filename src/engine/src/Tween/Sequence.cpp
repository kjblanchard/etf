#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/Tween/Tween.hpp>
using namespace Supergoon;

void Sequence::Update() {
	for (auto &&tween : Tweens) {
		if (tween->Complete()) {
			continue;
		}
		tween->Update();
		break;
	}
}
