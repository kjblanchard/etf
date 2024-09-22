
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>

#include <Supergoon/Game.hpp>
#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <istream>
#include <string>
using json = nlohmann::json;
using namespace Supergoon;

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;
sgBgm *_bgm;
sgSfx *sfx;
bool bgmLoaded = false;
Game *game;
bool _gameInitialized = false;

SDL_AppResult SDL_AppInit(void **, int, char *[]) {
	char *jsonPath = NULL;
	SDL_asprintf(&jsonPath, "%sassets/config.json", SDL_GetBasePath());
	std::ifstream fileStream(jsonPath);
	SDL_free(jsonPath);
	auto j = json::parse(fileStream);
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, 0, &window, &renderer)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
	if (!game) {
		return SDL_APP_CONTINUE;
	}
	if (!_gameInitialized) {
		game->Initialize();
		game->Start();
		_gameInitialized = true;
	}
	game->Update(0);
	SDL_RenderClear(renderer);
	game->Draw();
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *) {
}
Game::Game() {
	SDL_assert(!game);
	game = this;
}
void Game::Initialize() {
}