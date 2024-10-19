#pragma once
#include <Supergoon/World/Level.hpp>
namespace Supergoon {
struct GameState {
	int PlayerSpawnLocation;
	int WindowHeight;
	int WindowWidth;
	Level* CurrentLevel;
};
}  // namespace Supergoon
