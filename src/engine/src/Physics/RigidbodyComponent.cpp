#include <box2d/box2d.h>

#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
#define SCREENHEIGHT 288
RigidbodyComponent::RigidbodyComponent(Level& id, gePoint size, Vector2 loc) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position = SdlLocToBoxVec(loc.X, loc.Y);
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0.0f;
	bodyDef.linearDamping = 1.5f;
	bodyDef.angularDamping = 1.5f;
	_body = b2CreateBody(id.Physics().Id(), &bodyDef);
	_size.x = size.x;
	_size.y = size.y;
	b2Polygon dynamicBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.5f;
	b2CreatePolygonShape(_body, &shapeDef, &dynamicBox);
}
RigidbodyComponent::~RigidbodyComponent() {
}
Vector2 RigidbodyComponent::Location() {
	return BoxToSdlVector(b2Body_GetPosition(_body));
}
gePoint RigidbodyComponent::Size() {
	return gePoint{_size.x, _size.y};
}
void RigidbodyComponent::SetVelocity(Vector2 vel) {
	b2Body_SetLinearVelocity(_body, b2Vec2{vel.X, vel.Y});
}

void RigidbodyComponent::ApplyForceToBody(Vector2 force) {
	b2Body_ApplyForceToCenter(_body, b2Vec2{force.X, force.Y}, true);
}
