#pragma once
#include <Supergoon/Interfaces/Loader.hpp>
#include <Supergoon/Interfaces/Unloader.hpp>
#include <string>
namespace Supergoon {
class Content : public Loader, public Unloader {
   public:
	Content(std::string contentPath);
	virtual ~Content() = default;
	inline virtual const std::string Filepath() { return _filePath; }
	void LoadContent();
	void UnloadContent();
	inline virtual std::string Type() { return "Content"; }

   protected:
	std::string _filePath;
	bool _isLoaded = false;
};
}  // namespace Supergoon
