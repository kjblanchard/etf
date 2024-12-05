#include <SDL3/SDL_filesystem.h>

#include <Supergoon/Content/Font.hpp>
#include <Supergoon/Log.hpp>
using namespace Supergoon;
FT_Library Font::_loadedLibrary = nullptr;

Font::Font(std::string name, int size) : Content(name + std::to_string(_size)), _size(size) {}
Font::~Font() {
	Unload();
}

void Font::Load() {
	if (!_loadedLibrary) {
		if (FT_Init_FreeType(&_loadedLibrary)) {
			sgLogCritical("Could not initialize FreeType library\n");
		}
	}
	if (_size > 999 || _size < 1) {
		sgLogWarn("Improper size passed into font, must be between 1 and 1000, setting to 32.");
		_size = 32;
	}
	int result = FT_New_Face(_loadedLibrary, Filepath().c_str(), 0, &_face);

	if (result) {
		sgLogError("Could not open font %s with error %d\n", _filePath.c_str(), result);
		return;
	}
	// int dpi = 96;
	// int point_size = _size;	 // Your `_size` is the desired point size
	// FT_Set_Char_Size(_face, 0, point_size * 64, dpi, dpi);
	FT_Set_Pixel_Sizes(_face, 0, _size);
}

void Font::Unload() {
	if (_face) {
		FT_Done_Face(_face);
		_face = nullptr;
	}
}

const std::string Font::Filepath() {
	return std::string(SDL_GetBasePath()) + "assets/fonts/" + _filePath + ".ttf";
}
