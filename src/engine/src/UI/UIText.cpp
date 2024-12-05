#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/UI/UIText.hpp>
using namespace Supergoon;

// TODO, not sure why this is needed
static const int _centerYOffsetCorrection = 2;

UIText::UIText(Panel* parent, std::string text, std::string uiName, unsigned int size) : UIObject(parent), DisplayText(text) {
	auto uiname = uiName.empty() ? uiName : text;
	parent->Children[uiName] = std::shared_ptr<UIObject>(this);
	WidgetType = (int)BuiltinWidgetTypes::Text;
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", size);
	// Start the bounds to be the size of textptr, for ease of use.
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
	// TextPtr->SetTextBounds({(int)Bounds.W, (int)Bounds.H});
	_currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}

void UIText::Draw() {
	// We always want to draw the full text, if possible, otherwise we should cut off.
	// Src rect should either be the full text, or the size of the text that we decide.
	// Dst rect should always be the size of the src rect.
	TextPtr->Draw(TextSrcRect, TextDrawRect);
}

void UIText::OnDirty() {
	// TextPtr->LoadContent();
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	// Figure out the new src rect.
	// If we don't have text bounds, then we should just use the bounds x/y and size of text
	// if loaded do these things
	TextPtr->SetTextBounds({(int)Bounds.W, (int)Bounds.H});
	TextPtr->SetWordWrap(WordWrap);
	// Load before getting the bounds if this is a new one
	TextPtr->LoadContent();
	TextPtr->SetLetterCount(_currentLetters);
	TextPtr->SetAlpha(EffectiveAlpha());
	// If our bounds are set to 0, then we should use the full size.
	if (Bounds.W == 0 && Bounds.H == 0) {
		TextSrcRect = RectangleF{0, 0, (float)TextPtr->Size().X, (float)TextPtr->Size().Y};
	} else {
		// We want to use the whichever is smaller if we have a text bounds set.
		auto width = Bounds.W ? std::min((int)Bounds.W, TextPtr->Size().X) : TextPtr->Size().X;
		auto height = Bounds.H ? std::min((int)Bounds.H, TextPtr->Size().Y) : TextPtr->Size().Y;
		TextSrcRect = RectangleF{0, 0, (float)width, (float)height};
	}
	auto x = (int)Bounds.X;
	auto y = (int)Bounds.Y;
	if (CenterTextX) {
		x = (int)(Bounds.X + ((Bounds.W / 2) - (TextPtr->Size().X / 2)));
	}
	if (_centerTextY) {
		y = (int)(Bounds.Y + ((Bounds.H / 2) - (TextPtr->Size().Y / 2) - _centerYOffsetCorrection));
	}
	TextDrawRect = RectangleF{(float)x, (float)y, TextSrcRect.W, TextSrcRect.H};
}

void UIText::UpdateText(std::string text) {
	if (text == DisplayText || !TextPtr) {
		return;
	}
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", 16);
	_currentLetters = TextPtr->_lettersToDraw;
	Dirty = true;
}
