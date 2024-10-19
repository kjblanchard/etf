#pragma once

#include <Supergoon/Primitives/Point.hpp>
#include <Supergoon/Primitives/Vector2.hpp>

namespace Supergoon {
class Level;
struct StaticSolidComponent {
   public:
	StaticSolidComponent(Level& level, Point size = Point{0, 0}, Vector2 loc = Vector2(0));
	~StaticSolidComponent();
	Vector2 Location();
	Point Size();

   private:
	Point _size;
};
}  // namespace Supergoon
