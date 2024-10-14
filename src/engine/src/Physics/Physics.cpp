#include <box2d/box2d.h>
#include <Supergoon/Game.hpp>

#include <Supergoon/Physics/Physics.hpp>
using namespace Supergoon;

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
