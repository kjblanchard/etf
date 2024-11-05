#pragma once
namespace Supergoon {

class Loader {
   public:
	virtual ~Loader() = default;
	virtual void Load() = 0;
};
}  // namespace Supergoon
