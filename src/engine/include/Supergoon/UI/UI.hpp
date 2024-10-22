#pragma once
#include <string>
namespace Supergoon {

class Panel;
class UI {
   public:
	static Panel* LoadUIFromFile(std::string filename);
	static Panel* UIInstance;
};
}  // namespace Supergoon
