#pragma once
#include <Supergoon/Primitives/Point.h>
namespace Supergoon {
class Panel;

Panel* CreateUIBasePanel(std::string name, sgPoint screenLoc, sgPoint size, int alpha = 200);
// Screen is for the screen ui textbox I think?
Panel* CreateUITextbox(std::string name, sgPoint screenLoc, sgPoint size, bool screen);


}  // namespace Supergoon
