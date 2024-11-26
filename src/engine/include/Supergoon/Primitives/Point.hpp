#pragma once
namespace Supergoon {
struct Point {
	int X, Y;
	inline Point(int x, int y) : X(x), Y(y) {}
	inline Point() : X(0), Y(0) {}

	inline bool Zero() { return X && Y; }
};

}  // namespace Supergoon
