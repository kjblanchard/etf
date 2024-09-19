#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>

#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <istream>
#include <string>
using json = nlohmann::json;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
sgBgm *_bgm;
bool bgmLoaded = false;

SDL_AppResult SDL_AppInit(void **, int, char *[]) {
	char *jsonPath = NULL;
	SDL_asprintf(&jsonPath, "%sassets/config.json", SDL_GetBasePath());
	std::ifstream fileStream(jsonPath);
	SDL_free(jsonPath);
	auto j = json::parse(fileStream);
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, 0, &window, &renderer)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	_bgm = sgBgmNew();
	SDL_asprintf(&_bgm->Filename, "%sassets/town2.ogg", SDL_GetBasePath());
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
	if (!bgmLoaded) {
		sgBgmLoad(_bgm);
		sgBgmPlay(_bgm);
		bgmLoaded = true;
	}
	sgBgmUpdate(_bgm);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *) {
	sgBgmDelete(_bgm);
}