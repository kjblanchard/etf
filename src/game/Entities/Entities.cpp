#include <Entities/Battle/BattleLocation.hpp>
#include <Entities/Battle/BattleZone.hpp>
#include <Entities/PlayerExit.hpp>
#include <Entities/PlayerStart.hpp>
#include <Entities/TextInteraction.hpp>
#include <Supergoon/ECS/Gameobject.hpp>
#include <Supergoon/World/TiledMap.hpp>
#include <functional>
#include <unordered_map>
using namespace std;

namespace Supergoon {
std::unordered_map<std::string, std::function<GameObject *(TiledMap::TiledObject &)>> GameSpawnMap = {
    {"Start", [](TiledMap::TiledObject &object) {
       return NewPlayerSpawn(object);
     }},
    {"Exit", [](TiledMap::TiledObject &object) {
       return NewPlayerExit(object);
     }},
    {"BattleZone", [](TiledMap::TiledObject &object) {
       return NewBattleZone(object);
     }},
    {"BattleLocation", [](TiledMap::TiledObject &object) {
       return NewBattleLocation(object);
     }},
    {"TextInteract", [](TiledMap::TiledObject &object) {
       return NewTextInteraction(object);
     }}};
}
