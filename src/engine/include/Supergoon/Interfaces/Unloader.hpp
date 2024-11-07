
#pragma once
namespace Supergoon {

class Unloader {
   public:
	virtual ~Unloader() = default;
	virtual void Unload() = 0;
};
}  // namespace Supergoon
