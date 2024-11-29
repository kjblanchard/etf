#pragma once
#include <Supergoon/World/Level.hpp>
namespace Supergoon {
struct GameState {
	struct Battle {
		int BattleID;
	};
	Battle BattleData;
	int PlayerSpawnLocation;
	int WindowHeight;
	int WindowWidth;
	bool Loading;
	bool CameraFollowTarget;
	bool EnteringBattle;
	bool Interacting;
	bool InBattle;
};
}  // namespace Supergoon
