#pragma once

#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>

namespace Supergoon {
class Sound;
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
	inline Sound& Sound() { return *_bgm; }

	inline SDL_Window* Window() { return window; }
	inline SDL_Renderer* Renderer() { return renderer; }
	SDL_Window* window;
	SDL_Renderer* renderer;

   private:
	class Sound* _bgm = nullptr;
};
}  // namespace Supergoon
