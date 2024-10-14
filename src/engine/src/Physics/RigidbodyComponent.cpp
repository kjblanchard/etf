#include <box2d/box2d.h>

#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/RigidbodyComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
#define SCREENHEIGHT 288
RigidbodyComponent::RigidbodyComponent(Level& id, Vector2 loc) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	// bodyDef.type = b2_dynamicBody;	// Dynamic allows movement
	bodyDef.position = SdlLocToBoxVec(0, 0);
	bodyDef.type = b2_dynamicBody;
	bodyDef.gravityScale = 0.0f;
	bodyDef.linearDamping = 1.5f;
	bodyDef.angularDamping = 1.5f;
	_body = b2CreateBody(id.Physics().Id(), &bodyDef);
	b2Polygon dynamicBox = b2MakeBox(1.0f, 1.0f);
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

b2Vec2 RigidbodyComponent::SdlLocToBoxVec(int x, int y) {
	b2Vec2 box2dPoint;
	box2dPoint.x = static_cast<float>(x);
	box2dPoint.y = static_cast<float>(SCREENHEIGHT - y);
	return box2dPoint;
}
Vector2 RigidbodyComponent::BoxToSdlVector(b2Vec2 loc) {
	// gePoint sdlPoint;
	// sdlPoint.x = static_cast<int>(x);
	// sdlPoint.y = static_cast<int>(SCREENHEIGHT - y);  // screenHeight is the height of your SDL window
	// return sdlPoint;
	return Vector2(loc.x, (float)(int)SCREENHEIGHT - loc.y);
}
void RigidbodyComponent::ApplyForceToBody(Vector2 force) {
	b2Body_ApplyForceToCenter(_body, b2Vec2{force.X, force.Y}, true);
}
