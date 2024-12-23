#include <Components/BattleComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Entities/Battle/BattleState.hpp>
#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Sfx.hpp>
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
  Events::RegisterEventHandler(EscapeTheFateEvents.BattleDamageEvent, [](int battlerId, void *damage, void *) {
    GameObject::ForEach<BattlerComponent>([battlerId, &damage](GameObject, BattlerComponent &battlerComponent) {
      if (battlerComponent.Id != battlerId) {
        return;
      }
      // assert((u_int64_t)damage && "Cannot convert damage to int");
      // auto damageInt = (u_int64_t)damage;
      auto damageInt = 1;
      battlerComponent.Stat.HP -= damageInt;
      // If it's a enemy, we should play a sound and then victory.
      if (battlerComponent.Stat.HP <= 0) {
        Sound::Instance()->PlaySfx(enemyDiedSfx.get());
        enemyDiedTween.Restart();
      }
      // TODO If it's a player, we should update the hp
    });
  });
  enemyDiedTween = Tween(1.0);
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
  enemyDiedSfx = ContentRegistry::CreateContent<Sfx>("enemyDead");
  enemyDiedSfx->LoadContent();
}
void Supergoon::UpdateBattleDamageSystem() {
  enemyDiedTween.Update();
}
