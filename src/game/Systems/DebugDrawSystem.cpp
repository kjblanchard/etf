#include <Supergoon/Supergoon.hpp>
#include <Systems/DebugDrawSystem.hpp>
using namespace Supergoon;



static void drawStaticBodyDebugBoxes(GameObject, LocationComponent& location, StaticSolidComponent& sb) {
	// Get the camera and its position/box
	auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
	auto& cc = c->GetComponent<CameraComponent>();

	// Get the static body position (centered) and size
	Vector2 bodyPos = location.Location;  // Assuming this is the center of the physics body
	Point size = sb.Size();			  // Size of the static body (in pixels)

	// Adjust for camera position (subtract camera's top-left corner from body's position)
	float adjustedX = bodyPos.X - cc.Box.X;
	float adjustedY = bodyPos.Y - cc.Box.Y;

	// Correct for center-origin by subtracting half the width/height of the body
	adjustedX -= size.X / 2.0f;
	adjustedY -= size.Y / 2.0f;

	// Create the destination rectangle (aligned with top-left corner)
	auto dst = RectangleF{adjustedX, adjustedY, (float)size.X, (float)size.Y};

	// Draw the rectangle (debug visual)
	auto graphics = Graphics::Instance();
	graphics.DrawRect(dst, Color{255, 0, 0, 255});
}

void Supergoon::DrawDebugBoxes() {
	GameObject::ForEach<LocationComponent, StaticSolidComponent>(drawStaticBodyDebugBoxes);
}
