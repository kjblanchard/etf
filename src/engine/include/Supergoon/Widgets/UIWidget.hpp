#pragma once
#include <string>
namespace Supergoon {
class Panel;
class UIWidget {
   public:
	static void ShowUiDebugWindow();

   private:
	static void DrawPanel(Panel* panel, std::string panelName);
};

}  // namespace Supergoon
