#include <Components/PlayerComponent.hpp>
#include <Components/PlayerExitComponent.hpp>
#include <Supergoon/Supergoon.hpp>
#include <Systems/DebugDrawSystem.hpp>
using namespace Supergoon;
static void drawPlayerBodyDebugBoxes(GameObject, LocationComponent& location, PlayerComponent& sb) {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();
	Vector2 bodyPos = location.Location;  // Assuming this is the center of the physics body
	float adjustedX = bodyPos.X - cc.Box.X;
	float adjustedY = bodyPos.Y - cc.Box.Y;
	adjustedX += sb.Body.X;
	adjustedY += sb.Body.Y;
	auto dst = RectangleF{adjustedX, adjustedY, (float)sb.Body.W, (float)sb.Body.H};
	auto graphics = Graphics::Instance();
	graphics->DrawRect(dst, Color{255, 0, 255, 255});
}

static void drawStaticBodyDebugBoxes(GameObject, LocationComponent& location, SolidComponent& sb) {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();
	Vector2 bodyPos = location.Location;  // Assuming this is the center of the physics body
	Point size = sb.Size;				  // Size of the static body (in pixels)

	float adjustedX = bodyPos.X - cc.Box.X;
	float adjustedY = bodyPos.Y - cc.Box.Y;
	auto dst = RectangleF{adjustedX, adjustedY, (float)size.X, (float)size.Y};
	auto graphics = Graphics::Instance();
	graphics->DrawRect(dst, Color{255, 0, 0, 255});
}
static void drawPlayerExitDebugBoxes(GameObject, PlayerExitComponent& pe) {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();

	float adjustedX = pe.BoundingBox.X - cc.Box.X;
	float adjustedY = pe.BoundingBox.Y - cc.Box.Y;
	auto dst = RectangleF{adjustedX, adjustedY, (float)pe.BoundingBox.W, (float)pe.BoundingBox.H};
	auto graphics = Graphics::Instance();
	graphics->DrawRect(dst, Color{0, 255, 0, 255});
}
static void drawPlayerInteractionDebugBoxes(GameObject, PlayerComponent& player) {
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();

	float adjustedX = player.InteractionRect.X - cc.Box.X;
	float adjustedY = player.InteractionRect.Y - cc.Box.Y;
	auto dst = RectangleF{adjustedX, adjustedY, (float)player.InteractionRect.W, (float)player.InteractionRect.H};
	auto graphics = Graphics::Instance();
	graphics->DrawRect(dst, Color{0, 255, 0, 255});
}

void Supergoon::DrawDebugBoxesPlayer() {
	GameObject::ForEach<LocationComponent, PlayerComponent>(drawPlayerBodyDebugBoxes);
}
void Supergoon::DrawDebugBoxesSolid() {
	GameObject::ForEach<LocationComponent, SolidComponent>(drawStaticBodyDebugBoxes);
}
void Supergoon::DrawDebugBoxesPlayerExit() {
	GameObject::ForEach<PlayerExitComponent>(drawPlayerExitDebugBoxes);
}

void Supergoon::DrawDebugBoxesPlayerInteractionBox() {
	GameObject::ForEach<PlayerComponent>(drawPlayerInteractionDebugBoxes);
}
