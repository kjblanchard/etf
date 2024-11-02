#include <SDL3/SDL_filesystem.h>

#include <Supergoon/Content/Font.hpp>
#include <Supergoon/Log.hpp>
using namespace Supergoon;

Font::Font(std::string name, int size) : Content(name), _size(size) {}

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
	FT_Set_Pixel_Sizes(_face, 0, _size);
}

void Font::Unload() {
	FT_Done_Face(_face);
}

const std::string Font::Filepath() {
	return std::string(SDL_GetBasePath()) + "assets/font/" + _filePath;
}
