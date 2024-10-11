#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <string>
namespace Supergoon {

class Graphics {
   public:
	void CreateWindow(int width, int height, std::string name);
	void InitializeImGui();
	void DrawStart();
	void DrawEnd();
	void DrawImGui();

   private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
};
}  // namespace Supergoon
