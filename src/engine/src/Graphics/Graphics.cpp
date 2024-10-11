#include <SupergoonEngine/log.h>
#ifdef imgui
#include <SupergoonEngine/imgui_impl_sdl3.h>
#include <SupergoonEngine/imgui_impl_sdlrenderer3.h>
#endif

#include <Supergoon/Graphics/Graphics.hpp>
using namespace Supergoon;
void Graphics::CreateWindow(int width, int height, std::string name) {
	if (!SDL_CreateWindowAndRenderer(name.c_str(), width, height, 0, &_window, &_renderer)) {
		sgLogCritical("Could not load window, error, %s", SDL_GetError());
	}
}
void Graphics::InitializeImGui() {
#ifdef imgui
	ImGui_ImplSDL3_InitForSDLRenderer(_window, _renderer);
	ImGui_ImplSDLRenderer3_Init(_renderer);
#endif
}

void Graphics::DrawStart() {
	SDL_RenderClear(_renderer);
#ifdef imgui
	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
#endif
}

void Graphics::DrawImGui() {
	// Draw imgui
#ifdef imgui
	ImGui::ShowDemoWindow();
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
#endif
	ImGui::Render();
	// Draw imgui
#ifdef imgui
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
#endif
}
void Graphics::DrawEnd() {
	SDL_RenderPresent(_renderer);

}
