#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/UI/UIText.hpp>
using namespace Supergoon;

UIText::UIText(Panel* parent, std::string text) : UIObject(parent), DisplayText(text) {
	WidgetType = (int)BuiltinWidgetTypes::Text;
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", 16);
	// Start the bounds to be the size of textptr, for ease of use.
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
	// TextBounds = TextPtr->TextBounds();
	currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}

void UIText::Draw() {
	//  TODO this probably shouldn't be here.
	TextPtr->LoadContent();
	// We always want to draw the full text, if possible, otherwise we should cut off.
	// Src rect should either be the full text, or the size of the text that we decide.
	// Dst rect should always be the size of the src rect.
	auto dst = RectangleF(Bounds.X, Bounds.Y, TextSrcRect.W, TextSrcRect.H);
	TextPtr->Draw(TextSrcRect, dst);
}

void UIText::OnDirty() {
	TextPtr->LoadContent();
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	// Figure out the new src rect.
	// If we don't have text bounds, then we should just use the bounds x/y and size of text
	TextPtr->SetAlpha(EffectiveAlpha());
	TextPtr->SetTextBounds({(int)Bounds.W, (int)Bounds.H});
	TextPtr->SetLetterCount(currentLetters);
	TextPtr->SetWordWrap(WordWrap);
	// If our bounds are set to 0, then we should use the full size.
	if (Bounds.W == 0 && Bounds.H == 0) {
		TextSrcRect = RectangleF(0, 0, TextPtr->Size().X, TextPtr->Size().Y);
	} else {
		// We want to use the whichever is smaller if we have a text bounds set.
		auto width = Bounds.W ? std::min((int)Bounds.W, TextPtr->Size().X) : TextPtr->Size().X;
		auto height = Bounds.H ? std::min((int)Bounds.H, TextPtr->Size().Y) : TextPtr->Size().Y;
		TextSrcRect = RectangleF(0, 0, width, height);
	}
}

void UIText::UpdateText(std::string text) {
	if (text == DisplayText || !TextPtr) {
		return;
	}
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", 16);
	TextPtr->LoadContent();
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
	// create new text?
	// TextBounds = TextPtr->TextBounds();
	currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}
