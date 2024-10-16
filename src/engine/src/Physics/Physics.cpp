#include <box2d/box2d.h>

#include <Supergoon/Game.hpp>
#include <Supergoon/Physics/Physics.hpp>
using namespace Supergoon;
#define SCREENHEIGHT 288

PhysicsWorld::PhysicsWorld(Vector2 gravity) : _gravity(gravity) {
	b2WorldDef world = b2DefaultWorldDef();
	world.gravity.x = _gravity.X;
	world.gravity.y = _gravity.Y;
	_worldId = b2CreateWorld(&world);
}
void PhysicsWorld::Update() {
	auto deltatime = (float)Game::Instance()->DeltaTime();
	b2World_Step(_worldId, deltatime, 4);
}

b2Vec2 Supergoon::SdlLocToBoxVec(int x, int y) {
	b2Vec2 box2dPoint;
	box2dPoint.x = static_cast<float>(x);
	box2dPoint.y = static_cast<float>(SCREENHEIGHT - y);
	return box2dPoint;
}
Vector2 Supergoon::BoxToSdlVector(b2Vec2 loc) {
	return Vector2(loc.x, (float)(int)SCREENHEIGHT - loc.y);
}
