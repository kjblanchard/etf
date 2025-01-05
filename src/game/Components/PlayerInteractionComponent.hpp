#pragma once
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <memory>
namespace Supergoon {
struct PlayerInteractionComponent {
  RectangleF InteractionRect = {0, 0, 0, 0};
  std::shared_ptr<Image> InteractionImage = nullptr;
  bool ImageShowing = false;
};
} // namespace Supergoon
