#pragma once
namespace Supergoon {

class Loader {
   public:
	~Loader() = default;
	virtual void Load() = 0;
};
}  // namespace Supergoon
