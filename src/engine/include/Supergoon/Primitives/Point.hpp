#pragma once
namespace Supergoon {
struct Point {
	int X, Y;

	inline bool Zero() { return X && Y; }
};

}  // namespace Supergoon
