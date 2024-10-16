#include <box2d/box2d.h>

#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/StaticSolidComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
StaticSolidComponent::StaticSolidComponent(Level& level, gePoint size, Vector2 loc) {
	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.position = SdlLocToBoxVec(loc.X, loc.Y);
	_body = b2CreateBody(level.Physics().Id(), &bodyDef);
	_size.x = size.x;
	_size.y = size.y;
	b2Polygon dynamicBox = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	b2CreatePolygonShape(_body, &shapeDef, &dynamicBox);
}
StaticSolidComponent::~StaticSolidComponent() {
}
Vector2 StaticSolidComponent::Location() {
	return BoxToSdlVector(b2Body_GetPosition(_body));
}
gePoint StaticSolidComponent::Size() {
	return gePoint{_size.x, _size.y};
}
