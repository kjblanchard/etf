#include <Components/PlayerComponent.hpp>
#include <Supergoon/ECS/Components/CameraComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/World/Level.hpp>
#include <Supergoon/pch.hpp>
namespace Supergoon {

void getFollowTarget(CameraComponent &cc) {
  GameObject::ForEach<PlayerComponent, LocationComponent>([&cc](GameObject, PlayerComponent &pc, LocationComponent &lc) {
    if (pc.PlayerNum != 0) {
      return;
    }
    cc.FollowTarget = &lc;
  });
}

void UpdateCamera() {
  auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
  auto g = GameObject::GetGameObjectWithComponents<GameState>();
  if (!c.has_value() || !g.has_value()) {
    return;
  }
  auto &cc = c->GetComponent<CameraComponent>();
  auto &gc = g->GetComponent<GameState>();
  if (!gc.CameraFollowTarget) {
    return;
  }
  if (!cc.FollowTarget) {
    getFollowTarget(cc);
  }
  auto pl = cc.FollowTarget;
  if (!pl) {
    sgLogError("Trying to follow a target, but no target is found!");
    Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void *)&cc.Box);
    return;
  }
  cc.Box.X = (int)(pl->Location.X - (gc.WindowWidth / 2.0f));
  cc.Box.Y = (int)(pl->Location.Y - (gc.WindowHeight / 2.0f));
  if (cc.Box.X < 0) {
    cc.Box.X = 0;
  } else if (cc.Box.X > cc.Bounds.X - gc.WindowWidth) {
    cc.Box.X = std::max(cc.Bounds.X - gc.WindowWidth, 0);
  }
  if (cc.Box.Y < 0) {
    cc.Box.Y = 0;
  } else if (cc.Box.Y > (cc.Bounds.Y - gc.WindowHeight)) {
    cc.Box.Y = cc.Bounds.Y - gc.WindowHeight;
  }
  Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void *)&cc.Box);
}
} // namespace Supergoon
