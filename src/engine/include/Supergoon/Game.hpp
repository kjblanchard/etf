#pragma once
#include <SupergoonEngine/clock.h>

#include <memory>

typedef union SDL_Event SDL_Event;

namespace Supergoon {
class Sound;
class Graphics;
class Events;
class Game;
}  // namespace Supergoon

#define REGISTER_GAME(DERIVED_GAME_CLASS) \
	extern "C" Game* sgRegisterGame() {   \
		return new DERIVED_GAME_CLASS();  \
	}
#ifdef __cplusplus
extern "C" {
#endif
// Registers the game, this function needs to be defined in the derived class of game, by using the REGISTER_GAME macro and is the only requirement to use the engine.
Supergoon::Game* sgRegisterGame();
#ifdef __cplusplus
}
#endif

namespace Supergoon {
class Game {
   public:
	virtual ~Game();
	//    Happens once before game start
	void Initialize();
	// Game internal Update, do not call
	void InternalUpdate();
	// Game internal Draw, do not call
	void InternalDraw();
	void InternalReset();
	int HandleEvent(SDL_Event* event);
	//  Happens once at game start
	virtual void Start() = 0;
	//  Happens every frame
	virtual void Update() = 0;
	//  Happens after update
	virtual void Draw() = 0;
	virtual void Reset() = 0;
	static double DeltaTime();
	static double DeltaTimeMS();

   protected:
   private:
	void InitializeImGui();
	bool _initialized = false;
	geClock _clock;
	std::unique_ptr<Sound> _sound = nullptr;
	std::unique_ptr<Graphics> _graphics = nullptr;
	std::unique_ptr<Events> _events = nullptr;
};
}  // namespace Supergoon
