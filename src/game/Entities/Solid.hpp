#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <Supergoon/World/TiledMap.hpp>

namespace Supergoon {
class GameObject;
GameObject* NewSolidObject(TiledMap::TiledObject&);
GameObject* NewSolidObject(RectangleF& r);
}  // namespace Supergoon
