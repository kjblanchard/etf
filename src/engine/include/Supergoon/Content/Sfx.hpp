#pragma once
#include <Supergoon/Content/Content.hpp>
typedef struct sgSfx sgSfx;
namespace Supergoon {
class Sfx : public Content {
   public:
	virtual void Load() override;
	virtual void Unload() override;
	sgSfx* SgSfx();
	// Sfx(std::string filename);
	Sfx(std::string filename);
	~Sfx();

   private:
	sgSfx* _sfx;
};
}  // namespace Supergoon
