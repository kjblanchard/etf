#pragma once
namespace Supergoon {
class Game;
class Widgets {
   public:
	static void ShowWidgets(Game* game);
	static void InitializeWidgets();

	static int GetDefaultWindowFlags();
};
}  // namespace Supergoon
