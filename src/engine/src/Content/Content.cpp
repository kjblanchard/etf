#include <Supergoon/Content/Content.hpp>
using namespace Supergoon;

Content::Content(std::string contentPath) : _filePath(contentPath) {}

void Content::LoadContent() {
	if (_isLoaded) {
		return;
	}
	Load();
	_isLoaded = true;
}

void Content::UnloadContent() {
	if (!_isLoaded) {
		return;
	}
	Unload();
	_isLoaded = false;
}
