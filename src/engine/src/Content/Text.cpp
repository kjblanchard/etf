#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Content/Text.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Log.hpp>
using namespace Supergoon;

Text::Text(std::string text, std::string fontName, int size) : Content(text), _size(size), _text(text) {
	_font = ContentRegistry::CreateContent<Font, int>(fontName, std::move(size));
	_font->LoadContent();
	_lettersToDraw = text.length();
	MeasureText();
	auto imageName = std::string(text.substr(0, 30)) + std::to_string(_size);
	_image = ContentRegistry::CreateContent<Image, int, int>(imageName, std::move(_boundingBox.W), std::move(_boundingBox.H));
	// TODO this shouldn't be 20.
	_boundingBox.X = 20;
	_boundingBox.Y = 20;
	DrawLettersToTextImage();
}

void Text::Load() {
}

void Text::Unload() {
}

Text::~Text() {
}
void Text::Draw() {
	RectangleF d;
	d.X = _boundingBox.X;
	d.Y = _boundingBox.Y;
	// Try setting to image size.
	d.W = _textBounds.X;
	d.H = _textBounds.Y;
	auto src = RectangleF();
	_image->Draw(src, d);
	// geImageDraw(t->Texture, NULL, &t->BoundingBox);
	// geImageDraw(t->Texture, NULL, &d);
	// graphics->DrawImage(*this, &src, &dst);
}

void Text::MeasureText() {
	auto fontFace = _font->FontFace();
	int maxWidth = _textBounds.X ? _textBounds.X : INT_MAX;
	int maxHeight = _textBounds.Y ? _textBounds.Y : INT_MAX;
	maxWidth -= _paddingR;
	auto textSize = Point();
	int currentWordLength = 0, currentWordLetters = 0;
	int ascenderInPixels = (fontFace->ascender * _size) / fontFace->units_per_EM;
	int descenderInPixels = (fontFace->descender * _size) / fontFace->units_per_EM;
	int lineSpace = (fontFace->height * _size) / fontFace->units_per_EM;
	int startLoc = ascenderInPixels + _paddingT;

	// int penX = 0, penY = startLoc;
	int penX = _paddingL;
	int penY = startLoc;
	for (size_t i = 0; i < _text.length(); i++) {
		char letter = _text[i];
		if (letter == '\n') {
			if (currentWordLength) {
				AddWordToLetterPoints(fontFace, i, currentWordLetters, penX, penY);
				penX += currentWordLength;
				if (penX > textSize.X) {
					textSize.X = penX;
				}
			}
			penX = _paddingL;
			penY += lineSpace;
			currentWordLength = 0;
			currentWordLetters = 0;
			continue;
		}
		int letterSize = GetLetterWidth(fontFace, letter);
		if (letter == ' ') {
			AddWordToLetterPoints(fontFace, i, currentWordLetters, penX, penY);
			penX += currentWordLength + letterSize;
			currentWordLength = 0;
			currentWordLetters = 0;
			continue;
		}
		if (CheckShouldWrap(penX, currentWordLength, letterSize, maxWidth)) {
			if (penX > textSize.X) {
				textSize.X = penX;
			}
			penX = _paddingL;
			penY += lineSpace;
		}
		currentWordLength += letterSize;
		++currentWordLetters;
	}
	if (currentWordLength) {
		AddWordToLetterPoints(fontFace, _text.length(), currentWordLetters, penX, penY);
		penX += currentWordLength;
	}
	textSize.X = std::max(textSize.X, penX);
	textSize.Y = penY - descenderInPixels;
	if (textSize.Y > maxHeight) {
		sgLogWarn("Your text overflowed through Y, please adjust your bounds else it will flow past");
	}
	_boundingBox.W = textSize.X;
	_boundingBox.H = textSize.Y;
}

int Text::GetLetterWidth(FT_Face fontFace, char letter) {
	int result = FT_Load_Char(fontFace, letter, FT_LOAD_DEFAULT);
	if (result) {
		sgLogError("Could not measure character properly.  Char %s, error %d", letter, result);
		return 0;
	}
	return fontFace->glyph->advance.x >> 6;
}

bool Text::CheckShouldWrap(int x, int wordLength, int glyphWidth, int maxX) {
	return x + wordLength + glyphWidth > maxX;
}

void Text::AddWordToLetterPoints(FT_Face fontFace, int wordEndPos, int wordLength, int penX, int penY) {
	int x = penX, y = penY, wordStartPos = wordEndPos - wordLength;
	for (size_t i = 0; i < wordLength; i++) {
		int wordI = wordStartPos + i;
		if (wordI >= _text.length()) {
			sgLogWarn("How is this possible?");
			return;
		}
		char letter = _text[wordI];
		Point p = Point();
		p.X = x;
		p.X -= GetKerning(fontFace, wordI);
		p.Y = y - GetLetterYBearing(fontFace, letter);
		_letterPoints.push_back(p);
		// t->LetterPoints[wordI] = p;
		int width = GetLetterWidth(fontFace, letter);
		x += width;
	}
}
int Text::GetKerning(FT_Face fontFace, int i) {
	if (_text.length() <= i) {
		return 0;
	}
	// FT_Face f = geFontGetFont(t->Font);
	if (!FT_HAS_KERNING(fontFace)) {
		return 0;
	}
	unsigned int glyph_index_c = FT_Get_Char_Index(fontFace, _text[i]);
	unsigned int glyph_index_n = FT_Get_Char_Index(fontFace, _text[i + 1]);
	FT_Vector delta;
	int result = FT_Get_Kerning(fontFace, glyph_index_c, glyph_index_n, FT_KERNING_DEFAULT, &delta);
	if (result) {
		sgLogError("Could not get kerning");
	}
	result = delta.x >> 6;
	if (delta.x != 0) {
		sgLogWarn("Wow theres actually kerning and this hasen't been tested");
	}
	return result;
}

int Text::GetLetterYBearing(FT_Face fontFace, char letter) {
	int result = FT_Load_Char(fontFace, letter, FT_LOAD_DEFAULT);
	if (result) {
		sgLogError("Could not measure character properly.  Char %s, error %d", letter, result);
		return 0;
	}
	return fontFace->glyph->metrics.horiBearingY >> 6;
}

void Text::DrawLettersToTextImage(int startLoc) {
	for (size_t i = startLoc; i < _lettersToDraw; i++) {
		auto letter = _text[i];
		if (letter == ' ' || letter == '\n') {
			continue;
		}
		auto letterContentName = letter + std::to_string(_size);

		if (!ContentRegistry::ContentExists(letterContentName)) {
			int result = FT_Load_Char(_font->FontFace(), letter, FT_LOAD_RENDER);
			if (result) {
				printf("Failed to load character %c with error code %d \n", letter,
					   result);
				continue;
			}
			CreateSurfaceForLetter(letterContentName, _font->FontFace(), 255, 255, 255);
		}
		auto letterImage = ContentRegistry::GetContent<Image>(letterContentName);
		auto r = RectangleF();
		r.X = _letterPoints[i].X;
		r.Y = _letterPoints[i].Y;
		r.W = letterImage->Width();
		r.H = letterImage->Height();
		auto dstRect = RectangleF();
		_image->DrawImageToImage(*letterImage, dstRect, r);
	}
}

void Text::CreateSurfaceForLetter(std::string name, FT_Face fontFace, int r, int g, int b) {
	if (fontFace->glyph->bitmap.width == 0 && fontFace->glyph->bitmap.rows == 0)
		return;

	// auto surface = SDL_CreateSurfaceFrom(width, height, format, pixels, pitch);
	auto surface = SDL_CreateSurfaceFrom(fontFace->glyph->bitmap.width,
										 fontFace->glyph->bitmap.rows,
										 SDL_PIXELFORMAT_INDEX8,
										 fontFace->glyph->bitmap.buffer,
										 8 * fontFace->glyph->bitmap.pitch / fontFace->glyph->bitmap.width);
	SDL_Palette palette;
	palette.colors = (SDL_Color*)alloca(256 * sizeof(SDL_Color));
	int numColors = 256;
	for (int i = 0; i < numColors; ++i) {
		palette.colors[i].r = r;
		palette.colors[i].g = g;
		palette.colors[i].b = b;
		palette.colors[i].a = (Uint8)(i);
	}
	palette.ncolors = numColors;

	SDL_SetPaletteColors(&palette, palette.colors, 0, palette.ncolors);
	SDL_SetSurfacePalette(surface, &palette);
	SDL_SetSurfaceColorKey(surface, true, 0);
	ContentRegistry::CreateContent<Image, SDL_Surface*>(name, std::move(surface));
}
