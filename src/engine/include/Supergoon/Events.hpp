#pragma once
#include <cstdint>
#include <functional>
#include <unordered_map>
typedef union SDL_Event SDL_Event;
namespace Supergoon {
using EventFunc = std::function<void(int, void*, void*)>;

struct BuiltinEventTypes {
	uint32_t ImGuiFocusedEvent;
	uint32_t LevelChangeEvent;
};
class Events {
   public:
	Events();
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
