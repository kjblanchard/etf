#pragma once
#include <Supergoon/World/Level.hpp>
namespace Supergoon {
struct GameState {
	int PlayerSpawnLocation;
	int WindowHeight;
	int WindowWidth;
	bool Loading;
	// Level* CurrentLevel;
};
}  // namespace Supergoon
