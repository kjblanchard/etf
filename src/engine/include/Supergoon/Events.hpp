#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>
typedef union SDL_Event SDL_Event;
namespace Supergoon {
class Game;
using EventFunc = std::function<void(int, void*, void*)>;

struct BuiltinEventTypes {
	uint32_t ImGuiFocusedEvent;
	uint32_t LevelChangeEvent;
	uint32_t ResetGameEvent;
	uint32_t PlayBgmEvent;
	// TODO this should go
	uint32_t PlayBgmSecondaryEvent;
	uint32_t StopBgmSecondaryEvent;
	// TODO end
	uint32_t UiFadeInStart;
	uint32_t UiFadeInEnd;
	uint32_t UiFadeOutStart;
	uint32_t UiFadeOutEnd;
	uint32_t UiDestroyObject;
	uint32_t GameObjectAdd;
	// TODO not really sure if I like this, may make it jittery and behind on camera updates?
	uint32_t CameraUpdate;
};
class Events {
   public:
	Events(Game* game);
	bool HandleEvent(SDL_Event* event);
	static uint32_t RegisterEvent();
	static void PushEvent(uint32_t eventType, int eventCode, void* data = nullptr, void* data2 = nullptr);
	static int RegisterEventHandler(uint32_t eventType, EventFunc func);
	static void RemoveEventHandler(uint32_t eventType, int func);
	static BuiltinEventTypes BuiltinEvents;

   private:
	static void HandleCustomEventHandlers(SDL_Event* event);
	static std::unordered_map<uint32_t, std::vector<EventFunc>> _eventHandlers;
	bool _isGameFocused = true;
};

}  // namespace Supergoon
