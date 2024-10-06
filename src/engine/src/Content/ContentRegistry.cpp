#include <Supergoon/Content/ContentRegistry.hpp>
using namespace Supergoon;
std::unordered_map<std::string, std::shared_ptr<Content>> ContentRegistry::_loadedContent = {};

void ContentRegistry::LoadContent(Content& content) {
	content.LoadContent();
}

void ContentRegistry::ClearStaleContent() {
	for (auto it = _loadedContent.begin(); it != _loadedContent.end();) {
		if (it->second.use_count() == 1) {
			it = _loadedContent.erase(it);	// erase returns next iterator
		} else {
			++it;
		}
	}
}

void ContentRegistry::LoadAllContent() {
	for (auto&& [key, value] : _loadedContent) {
		value->LoadContent();
	}
}

void ContentRegistry::DestroyAllContent() {
	for (auto&& [key, value] : _loadedContent) {
		value->UnloadContent();
	}
	_loadedContent.clear();
}
