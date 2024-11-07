#pragma once
#include <Supergoon/Content/Text.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
namespace Supergoon {
class UIText : public UIObject {
   public:
	UIText(Panel* parent, std::string text);
	virtual void Draw() override;
	virtual void OnDirty() override;
	void UpdateText(std::string text);
	std::shared_ptr<Text> TextPtr;
	int Transparency = 255;
	Point TextBounds = {0, 0};
	int currentLetters = 0;
	bool WordWrap;
	std::string DisplayText;

   private:
	friend class UIWidget;
};
}  // namespace Supergoon
