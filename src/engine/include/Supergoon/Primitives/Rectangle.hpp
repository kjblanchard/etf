#pragma once
#include <SDL3/SDL_rect.h>

namespace Supergoon {
struct Rectangle {
	int X, Y, W, H;
	inline bool Zero() { return (X == 0 && Y == 0 && W == 0 && H == 0); }
	operator SDL_FRect() const {
		return SDL_FRect{(float)X, (float)Y, (float)W, (float)H};
	}
	inline bool IsOverlap(Rectangle* rhs) {
		return SDL_HasRectIntersection((SDL_Rect*)this, (SDL_Rect*)rhs);
	}
	inline Rectangle GetOverlapRect(Rectangle* rhs) {
		if (!IsOverlap(rhs)) {
			return Rectangle();
		}
		Rectangle r = Rectangle();
		SDL_GetRectIntersection((SDL_Rect*)this, (SDL_Rect*)rhs, (SDL_Rect*)&r);
		return r;
	}
};

struct RectangleF {
	float X, Y, W, H;
	inline bool Zero() { return (X == 0 && Y == 0 && W == 0 && H == 0); }
	operator SDL_FRect() const {
		return SDL_FRect{X, Y, W, H};
	}
	inline bool IsOverlap(RectangleF* rhs) {
		return SDL_HasRectIntersectionFloat((SDL_FRect*)this, (SDL_FRect*)rhs);
	}
	inline RectangleF GetOverlapRect(RectangleF* rhs) {
		if (!IsOverlap(rhs)) {
			RectangleF r = {0, 0, 0, 0};
			return r;
		}
		RectangleF r;
		SDL_GetRectIntersectionFloat((SDL_FRect*)this, (SDL_FRect*)rhs, (SDL_FRect*)&r);
		return r;
	}
};

}  // namespace Supergoon
