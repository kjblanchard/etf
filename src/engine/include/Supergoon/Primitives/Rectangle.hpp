#pragma once
#include <SDL3/SDL_rect.h>

namespace Supergoon {
struct Rectangle {
	int X, Y, W, H;
	inline bool Zero() { return (X == 0 && Y == 0 && W == 0 && H == 0); }
	operator SDL_FRect() const {
		return SDL_FRect{(float)X, (float)Y, (float)W, (float)H};
	}
};

struct RectangleF {
	float X, Y, W, H;
	inline bool Zero() { return (X == 0 && Y == 0 && W == 0 && H == 0); }
	operator SDL_FRect() const {
		return SDL_FRect{X, Y, W, H};
	}
};

}  // namespace Supergoon
