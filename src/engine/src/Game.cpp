
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>
#include <SupergoonEngine/clock.h>
#ifdef imgui
#include <SupergoonEngine/imgui.h>
#include <SupergoonEngine/imgui_impl_sdl3.h>
#include <SupergoonEngine/imgui_impl_sdlrenderer3.h>
#endif

#include <Supergoon/Game.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <SupergoonEngine/json.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>

using json = nlohmann::json;
using namespace Supergoon;
Game *Game::_game = nullptr;
static Game *_gameInternal = nullptr;

SDL_AppResult SDL_AppInit(void **, int, char *[]) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	sgInitializeDebugLogFile();
	sgRegisterGame();
	_gameInternal->Initialize();
	_gameInternal->Start();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
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

SDL_AppResult SDL_AppIterate(void *) {
	_gameInternal->InternalUpdate();
	_gameInternal->InternalDraw();
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *) {
}

Game::Game() {
	SDL_assert(!Game::Instance());
	_game = this;
	_gameInternal = _game;
}

void Game::Initialize() {
	char *jsonPath = NULL;
	SDL_asprintf(&jsonPath, "%sassets/config.json", SDL_GetBasePath());
	std::ifstream fileStream(jsonPath);
	SDL_free(jsonPath);
	auto j = json::parse(fileStream);
	int windowWidth = j["window"]["x"];
	int windowHeight = j["window"]["y"];
	std::string windowTitle = j["window"]["title"];
	if (!SDL_CreateWindowAndRenderer(windowTitle.c_str(), windowWidth, windowHeight, 0, &Game::Instance()->window, &Game::Instance()->renderer)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't create window/renderer!", SDL_GetError(), NULL);
		sgLogCritical("Could not load window");
	}
	_sound = new class Sound();
	geClockStart(&_clock);
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
}

void Game::InternalUpdate() {
	geClockUpdate(&_clock);
	_sound->Update();
	Update();
}

void Game::InternalDraw() {
	SDL_RenderClear(_gameInternal->renderer);

	// Render imgui
#ifdef imgui
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
#endif
	Draw();

	// Draw imgui
#ifdef imgui
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Game::Instance()->Renderer());
#endif
	ImGui::Render();
	// Draw imgui
#ifdef imgui
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Game::Instance()->Renderer());
#endif
	SDL_RenderPresent(Game::Instance()->renderer);
}

double Game::DeltaTime() {
	return geClockGetUpdateTimeSeconds();
}
