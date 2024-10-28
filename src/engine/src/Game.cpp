
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

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
#include <Supergoon/Log.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/UI/UI.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <istream>
#include <string>

using json = nlohmann::json;
using namespace Supergoon;
// Game *Game::_game = nullptr;
// static Game *_gameInternal = nullptr;

SDL_AppResult SDL_AppInit(void **appState, int, char *[]) {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Couldn't initialize SDL!", SDL_GetError(), NULL);
		return SDL_APP_FAILURE;
	}
	sgInitializeDebugLogFile();
	geInitializeKeyboard();
	geInitializeJoysticks();

	auto game = sgRegisterGame();
	*appState = game;
	game->InternalReset();
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appState, SDL_Event *event) {
	return (SDL_AppResult)((Game *)appState)->HandleEvent(event);
}

SDL_AppResult SDL_AppIterate(void *appState) {
	geUpdateKeyboard();
	auto game = (Game *)appState;
	game->InternalUpdate();
	game->InternalDraw();
	geUpdateControllerLastFrame();
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appState, SDL_AppResult) {
	auto game = (Game *)appState;
	game->Reset();
}

Game::Game() {
	// SDL_assert(!Game::Instance());
	// _game = this;
	// _gameInternal = _game;
}
Game::~Game() {
	sgCloseDebugLogFile();
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
	_sound = std::make_unique<Sound>();
	_graphics = std::make_unique<Graphics>();
	_events = std::make_unique<Events>(this);
	_graphics->CreateWindow(windowWidth, windowHeight, windowTitle);
	_graphics->SetWindowScaling(worldWidth, worldHeight);
	geClockStart(&_clock);
	InitializeImGui();
	_graphics->InitializeImGui();
	_sound->InitializeSound();
	_initialized = true;
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

void Game::InternalReset() {
	Reset();
	UI::Reset();
	GameObject::ClearGameObjects();
	// ContentRegistry::DestroyAllContent();
	if (!_initialized) {
		Initialize();
	}
	UI::Initialize();
	Start();
}

double Game::DeltaTime() {
	return geClockGetUpdateTimeSeconds();
}

double Game::DeltaTimeMS() {
	return geClockGetUpdateTimeMilliseconds();
}
