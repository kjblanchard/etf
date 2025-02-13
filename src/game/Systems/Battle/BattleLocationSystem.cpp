#include <Components/BattleComponent.hpp>
#include <Components/BattleLocationComponent.hpp>
#include <Components/BattlerComponent.hpp>
#include <Components/EnemyBattlerBlinkComponent.hpp>
// TODO remove this..
#include <SDL3/SDL_filesystem.h>
#include <Supergoon/ECS/Components/AnimationComponent.hpp>
#include <Supergoon/ECS/Components/LocationComponent.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Filesystem.hpp>
#include <Supergoon/Log.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <Systems/Battle/BattleLocationSystem.hpp>
#include <string>
using namespace Supergoon;
using namespace std;
using json = nlohmann::json;
json battleStats;
json battleGroups;
static int battleGroup = 0;

void loadStats() {
  // TODO we should make this not have to include SDL_Getbasepath
  std::string filename = SDL_GetBasePath() + std::string("assets/battle/stats.json");
  auto fileStream = SafeLoadFile(filename);
  battleStats = json::parse(fileStream);
  filename = SDL_GetBasePath() + std::string("assets/battle/battleGroups.json");
  fileStream = SafeLoadFile(filename);
  battleGroups = json::parse(fileStream);
}

void loadBattlers(GameObject, BattleLocationComponent &battleLocation) {
  // we want to load in the battlers from the current battle group and the players.  For now, lets load only one player in 1
  if (battleLocation.BattleLocationId != 4 && battleLocation.BattleLocationId != 1) {
    return;
  }
  auto go = new GameObject();
  auto battleLocationLoc = LocationComponent();
  auto battlerAnimation = AnimationComponent();
  auto battlerComponent = BattlerComponent();
  battlerComponent.CurrentATB = 0;
  battlerComponent.CurrentBattleLocation = battleLocation.BattleLocationId;
  // TODO we should use the battle zone to get the ids.
  // id1 is player, id4 is blackbird for now.
  auto id = 0;
  if (battleLocation.BattleLocationId == 1) {
    id = 1;
    // TODO we need to script this somehow, added in a project card for anim transitions.
    battlerAnimation.OnAnimationEnd = [](AsepriteAnimation *anim, std::string animEnding) {
      if (animEnding == "slash2" || animEnding == "damage1") {
        anim->PlayAnimation("idleR");
      }
    };
  } else {
    auto jsonBattleGroupIter = battleGroups.find(to_string(battleGroup));
    // TODO we need to handle loading more than one enemy, instead of just doing this.
    if (jsonBattleGroupIter == battleGroups.end()) {
      sgLogError("Could not find enemy id for %d, what the", id);
    }
    auto jsonBattleGroupEnemies = *jsonBattleGroupIter;
    auto battlers = jsonBattleGroupEnemies.at("Battlers").get<vector<int>>();
    id = battlers[0];
    auto blinkComponent = EnemyBattlerBlinkComponent();
    blinkComponent.Blinks = 0;
    blinkComponent.CurrentTime = 0;
    go->AddComponent<EnemyBattlerBlinkComponent>(blinkComponent);
  }
  auto jsonStatIter = battleStats.find(to_string(id));
  if (jsonStatIter == battleStats.end()) {
    sgLogError("Could not find stat for %d, what the", id);
  }
  auto jsonStat = *jsonStatIter;
  battlerAnimation.AnimationName = jsonStat["AnimationName"];
  battlerComponent.Stat.Name = jsonStat.at("Name").get<string>();
  battlerComponent.Stat.HP = jsonStat["MaxHP"];
  battlerComponent.Stat.MaxHP = jsonStat["MaxHP"];
  battlerComponent.Stat.Str = jsonStat["Strength"];
  battlerComponent.Id = jsonStat["Id"];
  battlerComponent.FullATB = jsonStat["Speed"];
  battlerComponent.IsPlayer = id == 1 ? true : false;
  battlerAnimation.Offset = {0, 0};
  battlerAnimation.AnimationSpeed = 1.0;
  battleLocationLoc.Location.X = battleLocation.Location.X;
  battleLocationLoc.Location.Y = battleLocation.Location.Y;
  go->AddComponent<AnimationComponent>(battlerAnimation);
  go->AddComponent<BattlerComponent>(battlerComponent);
  go->AddComponent<LocationComponent>(battleLocationLoc);
  Events::PushEvent(Events::BuiltinEvents.GameObjectAdd, true, (void *)go);
}
void startBattlers(GameObject, BattlerComponent &battler, AnimationComponent &anim) {
  // TODO should be in json for these offsets, for now hardcode.
  if (battler.Id == 1) {
    anim.Offset.X = -21;
    anim.Offset.Y = -41;
    anim.OverrideDrawSize = {72, 72};
  } else {
    anim.Offset.X = -15;
    anim.Offset.Y = -26;
  }
  anim.Animation->PlayAnimation("idleR");
  anim.Playing = true;
}

void Supergoon::InitializeStats() {
  loadStats();
}

void Supergoon::LoadBattlers() {
  auto battleComponent = GameObject::FindComponent<BattleComponent>();
  assert(battleComponent && "No battle component found!");
  battleGroup = battleComponent->BattleId;
  GameObject::ForEach<BattleLocationComponent>(loadBattlers);
}

void Supergoon::StartBattlers() {
  GameObject::ForEach<BattlerComponent, AnimationComponent>(startBattlers);
}
