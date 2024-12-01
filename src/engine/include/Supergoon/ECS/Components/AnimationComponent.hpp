#pragma once

#include <Supergoon/Aseprite/AsepriteAnimation.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Point.hpp>
#include <memory>
#include <string>
#include <utility>
namespace Supergoon {
struct AnimationComponent {
	std::string AnimationName;
	float AnimationSpeed;
	Point Offset = Point();
	Point OverrideDrawSize = Point();
	bool Playing;
	std::shared_ptr<AsepriteAnimation> Animation;
	std::shared_ptr<Image> AnimationImage;
};
}  // namespace Supergoon
