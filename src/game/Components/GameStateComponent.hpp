#pragma once
#include <Supergoon/World/Level.hpp>
namespace Supergoon {
struct GameState {
	int PlayerSpawnLocation;
	Level* CurrentLevel;
};
}  // namespace Supergoon
