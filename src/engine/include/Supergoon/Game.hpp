#pragma once

namespace Supergoon {
class Bgm;
class Game {
   public:
	Game();
	//    Happens once before game start
	virtual void Initialize();
	//  Happens once
	virtual void Start() = 0;
	//  Happens every frame
	virtual void Update(double deltaTime) = 0;
	//  Happens after update
	virtual void Draw() = 0;
	void InternalUpdate();

	inline Bgm& Sound() { return *_bgm; }

   private:
	class Bgm* _bgm = nullptr;
};
}  // namespace Supergoon