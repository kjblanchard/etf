#include <SDL3/SDL_filesystem.h>

#include <Supergoon/Content/Font.hpp>
#include <Supergoon/Log.hpp>
#include <sstream>
using namespace Supergoon;
FT_Library Font::_loadedLibrary = nullptr;

// Font::Font(std::string name, int size) : Content(name + "_" + std::to_string(size)), _size(size) {}
Font::Font(std::string name, int size) : Content(name), _size(size) {
}
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
	// auto path = Filepath();
	// std::string delimiter = "_";
	// std::string token = Filepath().substr(0, Filepath().find(delimiter)) + ".ttf";
	int result = FT_New_Face(_loadedLibrary, Filepath().c_str(), 0, &_face);
	// int result = FT_New_Face(_loadedLibrary, token.c_str(), 0, &_face);

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
	std::string fontName;
	std::istringstream stream(_filePath);
	std::getline(stream, fontName, '_');
	std::string fullPath = std::string(SDL_GetBasePath()) + "assets/fonts/" + fontName + ".ttf";
	return fullPath;
}
