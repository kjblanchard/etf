
#pragma once
namespace Supergoon {

class Unloader {
   public:
	~Unloader() = default;
	virtual void Unload() = 0;
};
}  // namespace Supergoon
