#pragma once
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
namespace Supergoon {
struct PlayerInteractionComponent {
	RectangleF InteractionRect;
	std::shared_ptr<Image> InteractionImage;
	bool ImageShowing;
};
}  // namespace Supergoon
