#include <Components/BattleZoneComponent.hpp>
#include <Components/PlayerComponent.hpp>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Input.hpp>
#include <Systems/Battle/BattleZoneSystem.hpp>
using namespace Supergoon;

void updateBattleZones(GameObject go, PlayerComponent& player, LocationComponent& loc) {
	auto state = GameObject::GetGameObjectWithComponents<GameState>();
	auto& stateComponent = state->GetComponent<GameState>();
	assert(state.has_value() && state->HasComponent<GameState>());
	GameObject::ForEach<BattleZoneComponent>([&go, &stateComponent, &player, &loc](GameObject, BattleZoneComponent& zone) {
		if (stateComponent.EnteringBattle) {
			return;
		}
		auto playerRect = Rectangle(player.Body.X + loc.Location.X,
									player.Body.Y + loc.Location.Y,
									player.Body.W,
									player.Body.H);
		if (playerRect.IsOverlap(zone.BoundingBox)) {
			assert(go.HasComponent<AnimationComponent>());
			auto& anim = go.GetComponent<AnimationComponent>();
			stateComponent.CameraFollowTarget = false;
			stateComponent.EnteringBattle = true;
			Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void*)strdup("battle1"));
			anim.Playing = false;
			return;
		}
	});
}

void Supergoon::UpdateBattleZones() {
	GameObject::ForEach<PlayerComponent, LocationComponent>(updateBattleZones);
}
