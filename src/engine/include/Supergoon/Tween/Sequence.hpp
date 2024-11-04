#pragma once
#include <Supergoon/Tween/Tween.hpp>
#include <memory>
#include <vector>
namespace Supergoon {
class Sequence {
   public:
	void Restart();
	void Update();
	inline bool IsComplete() { return _complete; }
	std::vector<std::shared_ptr<Tween>> Tweens;

   private:
	bool _complete = false;
};
}  // namespace Supergoon
