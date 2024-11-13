#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Log.hpp>
#include <algorithm>
using namespace Supergoon;
std::unordered_map<std::string, std::weak_ptr<Content>> ContentRegistry::_loadedContent = {};

void ContentRegistry::LoadContent(Content& content) {
	content.LoadContent();
}

void ContentRegistry::ClearStaleContent(bool force) {
	for (auto it = _loadedContent.begin(); it != _loadedContent.end();) {
		auto ptr = it->second.lock();
		if (!ptr) {
			it = _loadedContent.erase(it);
		} else {
			++it;
		}
		// if(it->second.lock)
		// if (it->second.use_count() == 1) {
		// 	it = _loadedContent.erase(it);	// erase returns next iterator
		// } else {
		// 	++it;
		// }
	}
}

void ContentRegistry::LoadAllContent() {
	for (auto&& [key, value] : _loadedContent) {
		auto shared = value.lock();
		if (shared) {
			shared->LoadContent();
		}
		// if(value.lock())
		// value->LoadContent();
	}
}

void ContentRegistry::DestroyAllContent() {
	for (auto&& [key, value] : _loadedContent) {
		auto shared = value.lock();
		if (shared) {
			shared->UnloadContent();
		}
		// value->UnloadContent();
	}
	_loadedContent.clear();
}
