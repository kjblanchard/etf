#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/UI/UIText.hpp>
using namespace Supergoon;

UIText::UIText(Panel* parent, std::string text) : UIObject(parent), DisplayText(text) {
	WidgetType = (int)BuiltinWidgetTypes::Text;
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", 16);
	auto ref = TextPtr.use_count();
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
	TextBounds = TextPtr->TextBounds();
	currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}

void UIText::Draw() {
	//  TODO this probably shouldn't be here.
	auto ref = TextPtr.use_count();
	TextPtr->LoadContent();
	TextPtr->Draw(Bounds);
}

void UIText::OnDirty() {
	auto ref = TextPtr.use_count();
	TextPtr->LoadContent();
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	TextPtr->SetAlpha(EffectiveAlpha());
	TextPtr->SetTextBounds({(int)TextBounds.X, (int)TextBounds.Y});
	TextPtr->SetLetterCount(currentLetters);
	TextPtr->SetWordWrap(WordWrap);
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
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
	TextBounds = TextPtr->TextBounds();
	currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}
