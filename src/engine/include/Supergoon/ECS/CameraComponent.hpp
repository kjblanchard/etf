#pragma once
#include <SupergoonEngine/prim/point.h>

#include <Supergoon/Rectangle.hpp>
namespace Supergoon {
struct LocationComponent;

struct CameraComponent {
	RectangleF Box;
	gePoint Bounds;
	LocationComponent* FollowTarget;
};

}  // namespace Supergoon
