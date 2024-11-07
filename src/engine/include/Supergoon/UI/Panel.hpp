#pragma once
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
#include <unordered_map>
namespace Supergoon {
class Panel : public UIObject {
   public:
	Panel();
	Panel(Panel* parent);
	int Transparency = 0;
	std::unordered_map<std::string, std::shared_ptr<UIObject>> Children;

	void OnDirty() override;
	void Update() override;
	void Draw() override;
};
}  // namespace Supergoon
