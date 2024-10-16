#pragma once
#include <Supergoon/Rectangle.hpp>
#include <Supergoon/World/TiledMap.hpp>
// #include <BbAdventures/tiled/TiledMap.hpp>

namespace Supergoon {
class GameObject;
GameObject* NewSolidObject(TiledMap::TiledObject&);
GameObject* NewSolidObject(RectangleF& r);
}  // namespace Supergoon
