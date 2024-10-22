#pragma once
#include <Supergoon/Tween/Tween.hpp>
#include <memory>
#include <vector>
namespace Supergoon {
class Sequence {
   public:
	void Restart();
	void Update();
	std::vector<std::shared_ptr<Tween>> Tweens;
};
}  // namespace Supergoon
