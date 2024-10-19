
#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>
#include <SupergoonEngine/clock.h>
#include <SupergoonEngine/input/joystick.h>
#include <SupergoonEngine/input/keyboard.h>
#ifdef imgui
#include <SupergoonEngine/imgui/imgui.h>
#endif

#include <Supergoon/Events.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
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
	geInitializeKeyboard();
	geInitializeJoysticks();

	sgRegisterGame();
	_gameInternal->Initialize();
	_gameInternal->Start();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *, SDL_Event *event) {
	return (SDL_AppResult)_gameInternal->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void *) {
	geUpdateKeyboard();
	_gameInternal->InternalUpdate();
	_gameInternal->InternalDraw();
	geUpdateControllerLastFrame();
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
	int worldWidth = j["world"]["x"];
	int worldHeight = j["world"]["y"];
	std::string windowTitle = j["window"]["title"];
	_sound = new class Sound();
	_graphics = new Graphics();
	_events = new Events();
	_graphics->CreateWindow(windowWidth, windowHeight, windowTitle);
	_graphics->SetWindowScaling(worldWidth, worldHeight);
	geClockStart(&_clock);
	InitializeImGui();
	_graphics->InitializeImGui();
	Game::Instance()->Sound().InitializeSound();
}

int Game::HandleEvent(SDL_Event *event) {
	return _events->HandleEvent(event);
}

void Game::InitializeImGui() {
#ifdef imgui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	   // IF using Docking Branch
#endif
}

void Game::InternalUpdate() {
	geClockUpdate(&_clock);
	_sound->Update();
	Update();
}

void Game::InternalDraw() {
	_graphics->DrawStart();
	Draw();
	_graphics->DrawImGui();
	_graphics->DrawEnd();
}

double Game::DeltaTime() {
	return geClockGetUpdateTimeSeconds();
}

double Game::DeltaTimeMS() {
	return geClockGetUpdateTimeMilliseconds();
}
