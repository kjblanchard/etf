#pragma once
#include <Supergoon/Content.hpp>
typedef struct sgSfx sgSfx;
namespace Supergoon {
class Sfx : public Content {
   public:
	virtual void Load() override;
	sgSfx* SgSfx();
	Sfx(std::string filename);
	~Sfx();

   private:
	sgSfx* _sfx;
	// friend class Sound;
};
}  // namespace Supergoon