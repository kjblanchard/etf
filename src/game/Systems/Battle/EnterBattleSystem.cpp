#include <Components/BattleComponent.hpp>
#include <Supergoon/ECS/Components/CameraComponent.hpp>
#include <Supergoon/ECS/Components/GameStateComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Primitives/Color.h>
#include <Supergoon/Tween/Sequence.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/World/Level.hpp>
#include <Systems/Battle/EnterBattleSystem.hpp>
#include <Utilities/Events.hpp>

using namespace Supergoon;

static float colorFade = 255;
static Sequence *sequence = nullptr;
void Supergoon::InitializeEnterBattleSystem() {
  sequence = new Sequence();
  auto tweener = std::make_shared<Tween>(255, 0, 1.0, &colorFade, Supergoon::Easings::Linear);
  tweener->UpdateFunc = []() {
    auto cameraGO = GameObject::GetGameObjectWithComponents<CameraComponent>();
    auto &cameraComponent = cameraGO->GetComponent<CameraComponent>();
    auto battleComponent = GameObject::FindComponent<BattleComponent>();
    if (!battleComponent) {
      return;
    }
    // Handle this somewhere else, like battle system.
    if (battleComponent->EnteringBattle) {
      cameraComponent.Box.X -= 5;
      sgColor color = {(uint8_t)colorFade, (uint8_t)colorFade, (uint8_t)colorFade, (uint8_t)colorFade};
      Level::SetBackGroundColor(color);
      Events::PushEvent(Events::BuiltinEvents.CameraUpdate, true, (void *)&cameraComponent.Box);
      return;
    };
  };
  sequence->Tweens.push_back(tweener);
  auto waitTween = std::make_shared<Tween>(0.1);
  sequence->Tweens.push_back(waitTween);
  waitTween->EndFunc = []() {
    // auto c = GameObject::GetGameObjectWithComponents<CameraComponent>();
  };
}
void Supergoon::UpdateEnterBattleSystem() {
  //   If we notice that we should enter a battle, lets set it properly.
  GameObject::ForEach<GameState, BattleComponent>([](GameObject, GameState &gameState, BattleComponent &battleComponent) {
    if (battleComponent.EnteringBattle) {
      sequence->Update();
      if (sequence->IsComplete()) {
        gameState.Loading = true;
        Events::PushEvent(Events::BuiltinEvents.LevelChangeEvent, 0, (void *)strdup(("forest1")));
        sequence->Restart();
        battleComponent.EnteringBattle = false;
        battleComponent.InBattle = true;
        Events::PushEvent(EscapeTheFateEvents.EnterBattleFinished, 0);
        UI::FadeIn();
      }
    }
  });
}
