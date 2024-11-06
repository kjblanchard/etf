#pragma once
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Content/Font.hpp>
#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Primitives/Point.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <memory>
#include <vector>
namespace Supergoon {
class Text : public Content {
   public:
	Text(std::string text, std::string fontName, int size);
	~Text();
	void Draw(RectangleF& dst);
	inline Point Size() { return _textSize; }
	virtual void Load() override;
	virtual void Unload() override;
	void SetTextBounds(Point bounds);
	void SetLetterCount(int letters);
	void SetWordWrap(bool wordWrap);
	inline Point TextBounds() { return _textBounds; }
	inline virtual std::string Type() override { return "Text"; }

   private:
	void MeasureText();
	int GetLetterWidth(FT_Face fontFace, char letter);
	bool CheckShouldWrap(int x, int wordLength, int glyphWidth, int maxX);
	void AddWordToLetterPoints(FT_Face fontFace, int wordEndPos, int wordLength, int penX, int penY);
	int GetKerning(FT_Face fontFace, int i);
	int GetLetterYBearing(FT_Face fontFace, char letter);
	void CreateSurfaceForLetter(std::string name, FT_Face fontFace, int r, int g, int b);
	void DrawLettersToTextImage(int startLoc = 0);
	std::string _text;
	int _fontSize;
	bool _wordWrap = false;
	int _lettersToDraw;
	// int _currentLettersDrawn;
	int _paddingL = 0, _paddingR = 0, _paddingT = 0;  //,_paddingB = 0;
	std::shared_ptr<Font> _font;
	std::shared_ptr<Image> _image;
	Point _textBounds = {0, 0};
	Point _textSize = {0, 0};
	// Rectangle _boundingBox = {0, 0, 0, 0};
	std::vector<Point> _letterPoints;
	friend class UIWidget;
	friend class UIText;
};
}  // namespace Supergoon
