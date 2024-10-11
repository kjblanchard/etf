#pragma once
typedef SDL_Event SDL_Event;
namespace Supergoon {
class Events {
   public:
	bool HandleEvent(SDL_Event* event);
};

}  // namespace Supergoon
