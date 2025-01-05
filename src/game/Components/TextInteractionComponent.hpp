#pragma once
// #include <Supergoon/Content/Text.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <memory>
#include <string>
namespace Supergoon {
class Text;
struct TextInteractionComponent {
  RectangleF InteractionRect = {0, 0, 0, 0};
  bool InteractionPressed = false;
  std::string DisplayText = "";
  std::shared_ptr<Text> TextPtr = nullptr;
};

} // namespace Supergoon
