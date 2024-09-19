#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>

#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <istream>
using json = nlohmann::json;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
sgBgm *_bgm;

SDL_AppResult SDL_AppInit(void **, int, char *[]) {
	// Read config file for engine
	char *jsonPath = NULL;
	SDL_asprintf(&jsonPath, "%sassets/config.json", SDL_GetBasePath()); /* allocate a string of the full file path */
	std::ifstream fileStream(jsonPath);
	SDL_free(jsonPath); /* done with this string. */
	auto j = json::parse(fileStream);
	// End config file for engine

	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}

	/* we don't _need_ a window for audio-only things but it's good policy to have one. */
	if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, 0, &window, &renderer)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	// sgBgm bgm;
	_bgm = (sgBgm*)malloc(sizeof(*_bgm));
	char *bgmPath = NULL;
	SDL_asprintf(&bgmPath, "%sassets/town2.ogg", SDL_GetBasePath()); /* allocate a string of the full file path */
	_bgm->Filename = bgmPath;
	sgBgmInit(_bgm);

	// You put data at Sint16/mono/22050Hz, you get back data at Float32/stereo/48000Hz.
	return SDL_APP_CONTINUE; /* carry on with the program! */
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
	}
	return SDL_APP_CONTINUE; /* carry on with the program! */
}

SDL_AppResult SDL_AppIterate(void *) {
	sgBgmUpdate(_bgm);
	SDL_RenderClear(renderer);
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;  // Continue the program
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *) {
	/* SDL will clean up the window/renderer for us. */
}