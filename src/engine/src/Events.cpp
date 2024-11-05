#include <SDL3/SDL.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Events.hpp>
#include <Supergoon/Game.hpp>
#include <Supergoon/UI/UI.hpp>
#include <Supergoon/World/Level.hpp>
#include <algorithm>
#ifdef imgui
#include <SupergoonEngine/imgui/imgui_impl_sdl3.h>
#endif
using namespace Supergoon;

std::unordered_map<Uint32, std::vector<std::function<void(int, void*, void*)>>> Events::_eventHandlers;
BuiltinEventTypes Events::BuiltinEvents;

Events::Events(Game* game) {
	BuiltinEvents.ImGuiFocusedEvent = RegisterEvent();
	BuiltinEvents.LevelChangeEvent = RegisterEvent();
	BuiltinEvents.ResetGameEvent = RegisterEvent();
	BuiltinEvents.PlayBgmEvent = RegisterEvent();
	BuiltinEvents.UiFadeInStart = RegisterEvent();
	BuiltinEvents.UiFadeInEnd = RegisterEvent();
	BuiltinEvents.UiFadeOutStart = RegisterEvent();
	BuiltinEvents.UiFadeOutEnd = RegisterEvent();

	RegisterEventHandler(BuiltinEvents.ImGuiFocusedEvent, [this](int code, void*, void*) {
		this->_isGameFocused = code;
	});

	RegisterEventHandler(BuiltinEvents.ResetGameEvent, [game](int, void*, void*) {
		game->InternalReset();
	});

	RegisterEventHandler(BuiltinEvents.LevelChangeEvent, [](int shouldFade, void* levelName, void*) {
		auto level = (const char*)levelName;
		if (shouldFade) {
			Level::LoadNewLevelFade(level);
		} else {
			Level::LoadNewLevel(level);
		}
		SDL_free(levelName);
	});
}

bool Events::HandleEvent(SDL_Event* event) {
#ifdef imgui
	ImGui_ImplSDL3_ProcessEvent(event);
	if (!_isGameFocused) {
		// We should probably not pass in any inputs to the game if it isn't focused in the editor.
	}
#endif
	// 	geUpdateControllerLastFrame();
	if (event->type == SDL_EVENT_QUIT) {
		return SDL_APP_SUCCESS;
	}
	// 	geUpdateKeyboard();
	HandleCustomEventHandlers(event);
	return SDL_APP_CONTINUE;
}

Uint32 Events::RegisterEvent() {
	return SDL_RegisterEvents(1);
}
void Events::HandleCustomEventHandlers(SDL_Event* event) {
	if (_eventHandlers.find(event->type) == _eventHandlers.end()) {
		return;
	}
	auto vec = _eventHandlers[event->type];
	for (auto&& func : vec) {
		if (!func) continue;
		func(event->user.code, event->user.data1, event->user.data2);
	}
}

int Events::RegisterEventHandler(Uint32 eventType, EventFunc func) {
	if (_eventHandlers.find(eventType) == _eventHandlers.end()) {
		_eventHandlers[eventType] = {};
		_eventHandlers[eventType].push_back(func);
		return 0;
	}
	// Find first spot that is nullptr, and then assign there.
	auto vec = _eventHandlers[eventType];
	if (auto it = std::find(vec.begin(), vec.end(), nullptr); it != vec.end()) {
		*it = func;
		return std::distance(vec.begin(), it);
	} else {
		vec.push_back(func);
		return _eventHandlers.size() - 1;
	}
}

void Events::RemoveEventHandler(Uint32 eventType, int func) {
	if (_eventHandlers.find(eventType) == _eventHandlers.end()) {
		sgLogWarn("Trying to remove a event with a type that doesn't exist: %d", eventType);
		return;
	}
	auto vec = _eventHandlers[eventType];
	vec[func] = nullptr;
}

void Events::PushEvent(Uint32 eventType, int eventCode, void* data, void* data2) {
	SDL_Event event;
	SDL_zero(event);
	event.type = eventType;
	event.user.code = eventCode;
	event.user.data1 = data;
	event.user.data2 = data2;
	SDL_PushEvent(&event);
}
