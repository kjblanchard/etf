#pragma once
#include <Supergoon/Content/Text.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
namespace Supergoon {
class UIText : public UIObject {
   public:
	UIText(Panel* parent, std::string text, std::string uiName = "");
	virtual void Draw() override;
	virtual void OnDirty() override;
	inline void SetCenter(bool center) {
		CenterTextX = center;
		Dirty = true;
	}
	inline void SetWordWrap(bool wrap) {
		WordWrap = wrap;
		Dirty = true;
	}
	inline void SetCenterY(bool center) {
		_centerTextY = center;
		Dirty = true;
	}
	void UpdateText(std::string text);
	std::shared_ptr<Text> TextPtr;
	int currentLetters = 0;
	RectangleF TextDrawRect = RectangleF();
	RectangleF TextSrcRect = RectangleF();
	bool WordWrap;
	bool CenterTextX = false;
	std::string DisplayText;

   private:
	bool _centerTextY = false;

   private:
	friend class UIWidget;
};
}  // namespace Supergoon
