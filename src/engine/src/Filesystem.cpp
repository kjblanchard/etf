#include <Supergoon/Filesystem.hpp>
#include <Supergoon/Log.hpp>
using namespace Supergoon;

std::ifstream Supergoon::SafeLoadFile(std::string& name) {
	std::ifstream fileStream(name);
	fileStream.open(name, std::ios::in | std::ios::binary);
	if (!fileStream.is_open()) {
		sgLogCritical("Error: File does not exist or could not be opened: %s ", name.c_str());
	}

	fileStream.seekg(0, std::ios::end);
	if (fileStream.tellg() == 0) {
		sgLogCritical("File is empty: %s ", name.c_str());
	}

	fileStream.seekg(0, std::ios::beg);
	return fileStream;
}
