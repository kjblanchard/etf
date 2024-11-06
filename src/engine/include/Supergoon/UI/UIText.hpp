#pragma once
#include <Supergoon/Content/Text.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <string>
namespace Supergoon {
class UIText : public UIObject {
	UIText(Panel* parent, std::string text);
	virtual void Draw() override;
	virtual void OnDirty() override;
	std::shared_ptr<Text> TextPtr;
	int Transparency = 255;
};
}  // namespace Supergoon
