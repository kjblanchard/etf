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
	void Draw();
	virtual void Load() override;
	virtual void Unload() override;
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
	int _size;
	bool _wordWrap = false;
	int _lettersToDraw;
	int _currentLettersDrawn;
	int _paddingL, _paddingR, _paddingT, _paddingB;
	std::shared_ptr<Font> _font;
	std::shared_ptr<Image> _image;
	Point _textBounds;
	Rectangle _boundingBox;
	std::vector<Point> _letterPoints;
};
}  // namespace Supergoon
