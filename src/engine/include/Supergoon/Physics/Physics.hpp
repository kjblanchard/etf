#pragma once
#include <box2d/id.h>

#include <Supergoon/Vector2.hpp>
namespace Supergoon {

class PhysicsWorld {
   public:
	PhysicsWorld(Vector2 gravity = Vector2(0));
	inline b2WorldId Id() { return _worldId; }
	void Update();

   private:
	Vector2 _gravity;
	b2WorldId _worldId;
};

}  // namespace Supergoon
