#pragma once
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
#include <unordered_map>
namespace Supergoon {
class UIWidget;
class Panel : public UIObject {
   public:
	Panel();
	Panel(Panel* parent);
	std::unordered_map<std::string, std::shared_ptr<UIObject>> Children;

	void OnDirty() override;
	void Update() override;
	void Draw() override;
	friend UIWidget;
};
}  // namespace Supergoon
