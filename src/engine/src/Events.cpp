#include <SDL3/SDL.h>

#include <Supergoon/Events.hpp>
#ifdef imgui
#include <SupergoonEngine/imgui_impl_sdl3.h>
#endif
using namespace Supergoon;

bool Events::HandleEvent(SDL_Event* event) {
// 	geUpdateControllerLastFrame();
#ifdef imgui
	ImGui_ImplSDL3_ProcessEvent(event);
#endif

	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	// 	geUpdateKeyboard();
	return SDL_APP_CONTINUE;
}
