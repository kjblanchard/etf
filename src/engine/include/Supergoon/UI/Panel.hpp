#pragma once
#include <Supergoon/UI/UIObject.hpp>
#include <string>
#include <unordered_map>
namespace Supergoon {
class Panel : public UIObject {
   public:
	std::unordered_map<std::string, std::shared_ptr<UIObject>> Children;

	void OnDirty() override;
	void Update() override;
	void Draw() override;
};
}  // namespace Supergoon
