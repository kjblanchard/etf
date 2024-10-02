#include <Supergoon/Content.hpp>
using namespace Supergoon;
std::unordered_map<std::string, std::shared_ptr<Content>> Content::_loadedContent = {};
Content::Content(std::string contentPath) : _filePath(contentPath) {}
void Content::LoadAllContent() {
	for (auto &&[key, value] : _loadedContent) {
		value->LoadContent();
	}
}

void Content::LoadContent() {
	if (_isLoaded) {
		return;
	}
	Load();
	_isLoaded = true;
}
// void Content::AddToContent(std::string contentName) {
// 	_loadedContent[contentName] = std::shared_ptr<Content>(this);
// }