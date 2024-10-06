
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

Game *Game::_game = nullptr;
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
	if (Game::Instance() && _gameInitialized) {
		ImGui_ImplSDL3_ProcessEvent(event);
	}
#endif
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *) {
	if (!Game::Instance()) {
		return SDL_APP_CONTINUE;
	}
	if (!_gameInitialized) {
		if (!SDL_CreateWindowAndRenderer("examples/audio/load-wav", 640, 480, 0, &Game::Instance()->window, &Game::Instance()->renderer)) {
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
			return SDL_APP_FAILURE;
		}
#ifdef imgui

		ImGui::CreateContext();
		ImGuiIO &io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	   // IF using Docking Branch
		ImGui_ImplSDL3_InitForSDLRenderer(Game::Instance()->window, Game::Instance()->Renderer());
		ImGui_ImplSDLRenderer3_Init(Game::Instance()->Renderer());
#endif
		Game::Instance()->Sound().InitializeSound();
		Game::Instance()->Initialize();
		Game::Instance()->Start();
		_gameInitialized = true;
	}
	Game::Instance()->InternalUpdate();
	Game::Instance()->Update(0);
	SDL_RenderClear(Game::Instance()->renderer);
#ifdef imgui
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
#endif

	Game::Instance()->Draw();
	ImGui::Render();
#ifdef imgui
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Game::Instance()->Renderer());
#endif
	SDL_RenderPresent(Game::Instance()->renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *) {
}
Game::Game() {
	SDL_assert(!Game::Instance());
	_game = this;
	_bgm = new class Sound();
}
void Game::Initialize() {
}
void Game::InternalUpdate() {
	_bgm->Update();
}
