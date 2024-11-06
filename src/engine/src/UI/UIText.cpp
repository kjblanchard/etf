#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/UI/UIText.hpp>
using namespace Supergoon;

UIText::UIText(Panel* parent, std::string text) : UIObject(parent) {
	WidgetType = (int)BuiltinWidgetTypes::Text;
	TextPtr = ContentRegistry::CreateContent<Text, std::string, int>(text, "commodore", 16);
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
	TextBounds = TextPtr->TextBounds();
	currentLetters = TextPtr->_lettersToDraw;
	WordWrap = TextPtr->_wordWrap;
}

void UIText::Draw() {
	//  TODO this probably shouldn't be here.
	TextPtr->LoadContent();
	TextPtr->Draw(Bounds);
}

void UIText::OnDirty() {
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	TextPtr->UpdateTextBounds({(int)TextBounds.X, (int)TextBounds.Y});
	TextPtr->UpdateLetterCount(currentLetters);
	TextPtr->UpdateWordWrap(WordWrap);
	Bounds.W = TextPtr->Size().X;
	Bounds.H = TextPtr->Size().Y;
}
