
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>

#include <Supergoon/Game.hpp>
#include <Supergoon/Sound.hpp>
#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>

#ifdef imgui
#include <SupergoonEngine/imgui.h>
#include <SupergoonEngine/imgui_impl_sdl3.h>
#include <SupergoonEngine/imgui_impl_sdlrenderer3.h>
#endif
using json = nlohmann::json;
using namespace Supergoon;

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
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
#ifdef imgui
	if (game && _gameInitialized) {
		ImGui_ImplSDL3_ProcessEvent(event);
	}
#endif
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
		if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, 0, &game->window, &game->renderer)) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
			return SDL_APP_FAILURE;
		}
#ifdef imgui

		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	   // IF using Docking Branch
		ImGui_ImplSDL3_InitForSDLRenderer(game->window, game->Renderer());
		ImGui_ImplSDLRenderer3_Init(game->Renderer());
#endif
		game->Sound().InitializeSound();
		game->Initialize();
		game->Start();
		_gameInitialized = true;
	}
	game->InternalUpdate();
	game->Update(0);
	SDL_RenderClear(game->renderer);
#ifdef imgui
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
#endif

	game->Draw();
	ImGui::Render();
#ifdef imgui
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), game->Renderer());
#endif
	SDL_RenderPresent(game->renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *) {
}
Game::Game() {
	SDL_assert(!game);
	game = this;
	_bgm = new class Sound();
}
void Game::Initialize() {
}
void Game::InternalUpdate() {
	_bgm->Update();
}
