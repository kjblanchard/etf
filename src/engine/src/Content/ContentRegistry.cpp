#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Log.hpp>
#include <algorithm>
using namespace Supergoon;
std::unordered_map<std::string, std::shared_ptr<Content>> ContentRegistry::_loadedContent = {};

void ContentRegistry::LoadContent(Content& content) {
	content.LoadContent();
}

void ContentRegistry::ClearStaleContent(bool force) {
	// If there is a lot of stale content, clear it all.
	int count = std::count_if(_loadedContent.begin(), _loadedContent.end(),
							  [](const auto& pair) { return pair.second.use_count() <= 1; });
	if (!force && count < 20) {
		return;
	}
	sgLogWarn("Clearing stale content as stale content count is %d", count);
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
