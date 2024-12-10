#include <Supergoon/ECS/GameObject.h>

#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/Game.hpp>
#include <Systems/GameStateSystem.hpp>
using namespace Supergoon;
void Supergoon::UpdateGameState() {
	auto time = Game::DeltaTime();
	// GameObject::ForEach<GameState>([time](GameObject, GameState& state) {
	// 	state.DeltaTime = time;
	// });
}
