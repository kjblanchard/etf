#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>
#include <SupergoonEngine/clock.h>

#define REGISTER_GAME(DERIVED_GAME_CLASS)                \
	extern "C" void sgRegisterGame() {                   \
		Game::SetGameInstance(new DERIVED_GAME_CLASS()); \
	}
#ifdef __cplusplus
extern "C" {
#endif
void sgRegisterGame();
#ifdef __cplusplus
}
#endif

namespace Supergoon {
class Sound;
class Graphics;
class Events;
class Game {
   public:
	Game();
	//    Happens once before game start
	void Initialize();
	// Game internal Update, do not call
	void InternalUpdate();
	// Game internal Draw, do not call
	void InternalDraw();
	int HandleEvent(SDL_Event* event);
	//  Happens once at game start
	virtual void Start() = 0;
	//  Happens every frame
	virtual void Update() = 0;
	//  Happens after update
	virtual void Draw() = 0;
	inline Sound& Sound() { return *_sound; }
	static inline Game* Instance() { return _game; }
	static double DeltaTime();
	static inline void SetGameInstance(Game* game) { _game = game; };

   private:
	void InitializeImGui();
	bool _initialized = false;
	geClock _clock;
	class Sound* _sound = nullptr;
	Graphics* _graphics = nullptr;
	Events* _events = nullptr;
	static Game* _game;
};
}  // namespace Supergoon
