#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleCommandArgs.hpp>
#include <Entities/Battle/BattleState.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
#include <Supergoon/Coroutine.h>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Tween/Tween.hpp>
#include <Systems/Battle/BattleDamageSystem.hpp>
#include <Utilities/Events.hpp>
#include <memory>
using namespace Supergoon;
using namespace std;
static int _alphaTweenInt = 0;
static shared_ptr<Sfx> enemyDiedSfx = nullptr;
static Tween enemyDiedTween = Tween(1.0);
static void startVictory(BattleComponent *battleComponent) {
  Events::PushEvent(Events::BuiltinEvents.PlayBgmEvent, 0, (void *)strdup("victory"));
  GameObject::ForEach<BattlerComponent, AnimationComponent>([](GameObject, BattlerComponent &battler, AnimationComponent &anim) {
    if (!battler.IsPlayer) {
      return;
    }
    anim.Animation->PlayAnimation("cheer1");
  });
  Events::PushEvent(EscapeTheFateEvents.VictoryStart, 0);
  battleComponent->CurrentBattleState = BattleState::Victory;
}
void Supergoon::InitializeBattleDamageSystem(BattleComponent *battleComponent) {
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleDamageEvent, [battleComponent](int damage, void *abilityArgsVoid, void *) {
    assert((BattleCommandArgs *)abilityArgsVoid && "Could not convert properly~!");
    auto abilityArgs = (BattleCommandArgs *)abilityArgsVoid;
    auto battlerId = abilityArgs->TargetBattler.GetComponent<BattlerComponent>().Id;
    // auto targetAnim = abilityArgs->TargetBattler.GetComponent<AnimationComponent>();
    GameObject::ForEach<BattlerComponent, AnimationComponent>([battlerId, battleComponent](GameObject, BattlerComponent &battlerComponent, AnimationComponent &animComponent) {
      if (battlerComponent.Id != battlerId) {
        return;
      }
      auto damageInt = 1;
      battlerComponent.Stat.HP -= damageInt;
      // If it's a enemy, we should play a sound and then victory.
      if (battlerComponent.Stat.HP <= 0) {
        enemyDiedTween = Tween(255, 0, 0.75, &_alphaTweenInt, Easings::Linear, 1);
        enemyDiedTween.UpdateFunc = [animComponent]() {
          animComponent.AnimationImage->SetAlpha(_alphaTweenInt);
        };
        enemyDiedTween.SetAutostart(false);
        enemyDiedTween.EndFunc = [battleComponent]() {
          if (battleComponent->CurrentBattleState != BattleState::Battle) {
            return;
          }
          bool allEnemiesDead = true;
          GameObject::ForEach<BattlerComponent>([&allEnemiesDead](GameObject, BattlerComponent &battlerComp) {
            if (battlerComp.IsPlayer) {
              return;
            }
            if (battlerComp.Stat.HP > 0) {
              allEnemiesDead = false;
            }
          });
          if (allEnemiesDead) {
            // Should signal to start the victory to the battle system.
            startVictory(battleComponent);
          }
        };
        auto co = sgAddCoroutine(
            0.25, [](void *, void *) {
              Sound::Instance()->PlaySfx(enemyDiedSfx.get());
              enemyDiedTween.Restart();
            },
            nullptr, nullptr);
        sgStartCoroutine(co);
      }
      // TODO If it's a player, we should update the hp
    });
    delete (abilityArgs);
  });
  enemyDiedSfx = ContentRegistry::CreateContent<Sfx>("enemyDead");
  enemyDiedSfx->LoadContent();
}
void Supergoon::UpdateBattleDamageSystem() {
  enemyDiedTween.Update();
}
void Supergoon::EndBattleDamageSystem() {
  enemyDiedTween = Tween(1.0f);
}
