#pragma once
#include <Supergoon/Content/Content.hpp>
typedef struct sgSfx sgSfx;
namespace Supergoon {
class Sfx : public Content {
   public:
	virtual void Load() override;
	virtual void Unload() override;
	inline virtual std::string Type() override { return "Sfx"; }
	virtual const std::string Filepath() override;
	sgSfx* SgSfx();
	Sfx(std::string filename);
	~Sfx();

   private:
	sgSfx* _sfx;
};
}  // namespace Supergoon
