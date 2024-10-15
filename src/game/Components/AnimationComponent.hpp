#pragma once

#include <SupergoonEngine/prim/point.h>

#include <Supergoon/Aseprite/AsepriteAnimation.hpp>
#include <Supergoon/Content/Image.hpp>
#include <memory>
#include <string>
namespace Supergoon {
struct AnimationComponent {
	AsepriteAnimation* Animation;
	std::string AnimationName;
	float AnimationSpeed;
	Image* AnimationImage;
	gePoint Offset;
	bool Playing;
};
}  // namespace Supergoon
