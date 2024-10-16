#pragma once
#include <SupergoonEngine/prim/point.h>
#include <box2d/id.h>
#include <box2d/types.h>

#include <Supergoon/Vector2.hpp>

namespace Supergoon {
class Level;
struct StaticSolidComponent {
   public:
	StaticSolidComponent(Level& level, gePoint size = gePoint{0, 0}, Vector2 loc = Vector2(0));
	~StaticSolidComponent();
	Vector2 Location();
	gePoint Size();

   private:
	gePoint _size;
	b2BodyId _body;
};
}  // namespace Supergoon
