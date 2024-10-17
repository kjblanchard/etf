
#include <Supergoon/Physics/Physics.hpp>
#include <Supergoon/Physics/StaticObjectComponent.hpp>
#include <Supergoon/World/Level.hpp>
using namespace Supergoon;
StaticSolidComponent::StaticSolidComponent(Level& level, gePoint size, Vector2 loc) {
}
StaticSolidComponent::~StaticSolidComponent() {
}
Vector2 StaticSolidComponent::Location() {
}
gePoint StaticSolidComponent::Size() {
	return gePoint{_size.x, _size.y};
}
