#pragma once
#include <Supergoon/Content/Content.hpp>
#include <memory>
#include <unordered_map>

namespace Supergoon {

// Registry for the game, functions to create content and manage them.
class ContentRegistry {
   public:
	//  Force loads a specific content
	static void LoadContent(Content& content);
	//  Loads all content that isn't loaded
	static void LoadAllContent();
	// Clear any content that isn't being used, useful to run between loading levels
	static void ClearStaleContent();
	// Unloads all content and clears the loaded content list, even if shared ptrs still have references
	static void DestroyAllContent();
	/**
	 * @brief Creates a content type for you to use, all content types should be created with this to properly manage the content of the game.
	 *
	 * @tparam T type of content
	 * @param key the filepath for this
	 * @return std::shared_ptr<T> A shared_ptr to reference this content with
	 */
	template <typename T>
	static std::shared_ptr<T> CreateContent(const std::string& key) {
		auto it = _loadedContent.find(key);
		if (it != _loadedContent.end()) {
			std::shared_ptr<T> specificContent = std::dynamic_pointer_cast<T>(it->second);
			if (specificContent) {
				return specificContent;
			}
		}
		// If content doesn't exist or is expired, load it and store it in the map
		std::shared_ptr<T> newContent = std::make_shared<T>(key);
		_loadedContent[key] = newContent;
		return newContent;
	}

   private:
	static std::unordered_map<std::string, std::shared_ptr<Content>> _loadedContent;
};

}  // namespace Supergoon
