#pragma once
#include <Supergoon/Primitives/Point.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
struct LocationComponent;

struct CameraComponent {
	RectangleF Box;
	Point Bounds;
	LocationComponent* FollowTarget;
};

}  // namespace Supergoon
