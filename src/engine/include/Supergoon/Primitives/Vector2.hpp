#pragma once
namespace Supergoon {
struct Vector2 {
	inline Vector2() : X(0), Y(0){};
	inline Vector2(float xy) {
		X = xy;
		Y = xy;
	}
	inline Vector2(float x, float y) {
		X = x;
		Y = y;
	}
	inline bool operator==(const Vector2& rhs) const {
		return rhs.X == X && rhs.Y == Y;
	}
	inline void operator+=(const Vector2& rhs) {
		X += rhs.X;
		Y += rhs.Y;
	}
	inline void operator*=(const Vector2& rhs) {
		X *= rhs.X;
		Y *= rhs.Y;
	}
	float X = 0;
	float Y = 0;
};
}  // namespace Supergoon
