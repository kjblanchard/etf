#pragma once

#include <Supergoon/Aseprite/AsepriteAnimation.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Point.hpp>
#include <memory>
#include <string>
namespace Supergoon {
struct AnimationComponent {
	AsepriteAnimation* Animation;
	std::string AnimationName;
	float AnimationSpeed;
	std::shared_ptr<Image> AnimationImage;
	Point Offset;
	bool Playing;
};
}  // namespace Supergoon
