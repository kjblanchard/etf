#pragma once
#include <box2d/id.h>
#include <box2d/types.h>

#include <Supergoon/Vector2.hpp>

namespace Supergoon {
class Level;
class RigidbodyComponent {
   public:
	RigidbodyComponent(Level& level, Vector2 loc = Vector2(0));
	~RigidbodyComponent();
	Vector2 Location();
	void SetVelocity(Vector2 vel);
	void ApplyForceToBody(Vector2 force);

   private:
	b2Vec2 SdlLocToBoxVec(int x, int y);
	Vector2 BoxToSdlVector(b2Vec2 loc);
	b2BodyId _body;
};
}  // namespace Supergoon
