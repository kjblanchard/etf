#pragma once
// #include <Supergoon/Content/Text.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <memory>
#include <string>
namespace Supergoon {
class Text;
struct TextInteractionComponent {
	RectangleF InteractionRect;
	bool InteractionPressed;
	std::string DisplayText;
	std::shared_ptr<Text> TextPtr;
};

}  // namespace Supergoon
