#pragma once
#include <Supergoon/Content/Text.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <Supergoon/UI/UIObject.hpp>
#include <memory>
#include <string>
namespace Supergoon {
class UIText : public UIObject {
   public:
	UIText(UIObject* parent, std::string text, std::string uiName = "", unsigned int size = 16);
	// UIText(Panel* parent, std::string text, std::string uiName = "", unsigned int size = 16);
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
	inline void SetCurrentLetters(int letters) {
		_currentLetters = letters;
		Dirty = true;
	}
	inline int* CurrentLettersRef() { return &_currentLetters; }
	void UpdateText(std::string text);
	std::shared_ptr<Text> TextPtr;
	RectangleF TextDrawRect = RectangleF();
	RectangleF TextSrcRect = RectangleF();
	bool WordWrap;
	bool CenterTextX = false;
	std::string DisplayText;

   private:
	bool _centerTextY = false;
	int _currentLetters = 0;

   private:
	friend class UIWidget;
};
}  // namespace Supergoon
