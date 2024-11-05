#pragma once
#include <Supergoon/Tween/Tween.hpp>
#include <memory>
#include <vector>
namespace Supergoon {
class Sequence {
   public:
	void Restart();
	void Update();
	// Are all of the tweens in this sequence complete?
	inline bool IsComplete() { return _complete; }
	// percent 88/etc of duration for all tweens included.
	int Percent();
	// Tweens added to this sequence.
	std::vector<std::shared_ptr<Tween>> Tweens;

   private:
	bool _complete = false;
};
}  // namespace Supergoon
