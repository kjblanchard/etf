#pragma once
#include <string>
namespace Supergoon {

class Panel;
class UI {
   public:
	static Panel* Initialize();
	static void LoadUIFromFile(std::string filename, Panel* rootPanel);
	// static Panel* UIInstance;
};
}  // namespace Supergoon
