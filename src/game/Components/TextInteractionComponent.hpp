#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <string>
namespace Supergoon {
struct TextInteractionComponent {
	RectangleF InteractionRect;
	bool InteractionPressed;
	std::string Text;
};

}  // namespace Supergoon
