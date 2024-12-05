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
	Panel(Panel* parent, std::string name);
	std::unordered_map<std::string, std::shared_ptr<UIObject>> Children;

	void OnDirty() override;
	void Update() override;
	void Draw() override;

   private:
	std::vector<UIObject*> _drawOrder;
	friend UIWidget;
};
}  // namespace Supergoon
