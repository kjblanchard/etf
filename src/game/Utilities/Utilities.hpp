#pragma once
#include <Supergoon/Primitives/Point.hpp>
#include <string>
namespace Supergoon {
class Panel;

Panel* CreateUIBasePanel(std::string name, Point screenLoc, Point size, int alpha = 200);
// Screen is for the screen ui textbox I think?
Panel* CreateUITextbox(std::string name, Point screenLoc, Point size, bool screen);


}  // namespace Supergoon
