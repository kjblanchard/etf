
#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/StaticObjectComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
StaticSolidComponent::StaticSolidComponent(Level&, Point, Vector2) {
}
StaticSolidComponent::~StaticSolidComponent() {
}
Vector2 StaticSolidComponent::Location() {
	return Vector2();
}
Point StaticSolidComponent::Size() {
	return Point{_size.X, _size.Y};
}
