#pragma once
#include <Supergoon/Content/Image.hpp>
#include <memory>
namespace Supergoon {
struct ImageComponent {
	std::shared_ptr<Image> Image = nullptr;
	RectangleF SourceRect;
};
}  // namespace Supergoon
