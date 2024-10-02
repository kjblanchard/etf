#pragma once
#include <string>
#include <unordered_map>
namespace Supergoon {
class Content {
   public:
	Content(std::string contentPath);
	virtual ~Content() = default;
	virtual void Load() = 0;
	void LoadContent();
	static void LoadAllContent();

//    protected:
   public:
	std::string _filePath;
	bool _isLoaded = false;
	template <typename T>
	static std::shared_ptr<T> GetOrCreateContent(const std::string& key) {
		// Check if content exists in the map
		auto it = _loadedContent.find(key);
		if (it != _loadedContent.end()) {
			// Try to cast it to the requested type
			std::shared_ptr<T> specificContent = std::dynamic_pointer_cast<T>(it->second);
			if (specificContent) {
				return specificContent;
			}
		}

		// If content doesn't exist, load it and store it in the map
		std::shared_ptr<T> newContent = std::make_shared<T>(key);
		_loadedContent[key] = newContent;
		return newContent;
	}
	// void AddToContent(std::string contentName);

   private:
	static std::unordered_map<std::string, std::shared_ptr<Content>> _loadedContent;
};
}  // namespace Supergoon