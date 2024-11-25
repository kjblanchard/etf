#pragma once
#include <Supergoon/Primitives/Point.hpp>
namespace Supergoon {
class Panel;

Panel* CreateUITextbox(std::string name, Point screenLoc, Point size, bool screen);


}  // namespace Supergoon
