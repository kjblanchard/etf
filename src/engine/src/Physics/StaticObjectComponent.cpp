
#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/StaticObjectComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
StaticSolidComponent::StaticSolidComponent(Level& level, Point size, Vector2 loc) {
}
StaticSolidComponent::~StaticSolidComponent() {
}
Vector2 StaticSolidComponent::Location() {
}
Point StaticSolidComponent::Size() {
	return Point{_size.X, _size.Y};
}
