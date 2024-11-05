#pragma once
#include <ft2build.h>

#include <Supergoon/Content/Content.hpp>
#include FT_FREETYPE_H
namespace Supergoon {
class Font : public Content {
   public:
	Font(std::string name, int size);
	~Font();
	virtual void Load() override;
	virtual void Unload() override;
	virtual const std::string Filepath() override;
	inline virtual std::string Type() override { return "Font"; }
	inline FT_Face FontFace() { return _face; }

   private:
	//    TODO this is never really freed anywhere.
	static FT_Library _loadedLibrary;
	int _size;
	FT_Face _face;
};
}  // namespace Supergoon
