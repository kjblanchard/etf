#include <SDL3/SDL_filesystem.h>
#include <SupergoonEngine/Sfx.h>

#include <Supergoon/Content/Sfx.hpp>
using namespace Supergoon;
Sfx::Sfx(std::string filename) : Content(filename) {
	_sfx = sgSfxNew();
	auto fullFilename = SDL_GetBasePath() + std::string("assets/") + filename + ".ogg";
	_sfx->Filename = strdup(fullFilename.c_str());
}
sgSfx* Sfx::SgSfx() {
	return _sfx;
}

Sfx::~Sfx() {
	sgSfxDelete(_sfx);
}
void Sfx::Load() {
	sgSfxLoad(_sfx);
}
