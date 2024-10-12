#include <SupergoonEngine/log.h>

#include <Supergoon/Content/Image.hpp>
#ifdef imgui
#include <SupergoonEngine/imgui_impl_sdl3.h>
#include <SupergoonEngine/imgui_impl_sdlrenderer3.h>
#endif

#include <Supergoon/Graphics/Graphics.hpp>
using namespace Supergoon;
Graphics* Graphics::_instance = nullptr;
void Graphics::CreateWindow(int width, int height, std::string name) {
	auto flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY;
	if (!SDL_CreateWindowAndRenderer(name.c_str(), width, height, flags, &_window, &_renderer)) {
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
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), _renderer);
#endif
}
void Graphics::DrawEnd() {
	SDL_RenderPresent(_renderer);
}
void Graphics::DrawImage(Image& image, RectangleF* srcR, RectangleF* dstR) {
	SDL_RenderTexture(_renderer, image._image, (SDL_FRect*)srcR, (SDL_FRect*)dstR);
}
SDL_Texture* Graphics::CreateRenderTargetTexture(int width, int height, Color color) {
	auto image = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
	SDL_SetRenderTarget(_renderer, image);
	if (SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND) != true) {
		sgLogError("Error setting blend mode: %s", SDL_GetError());
	}
	SDL_SetTextureBlendMode(image, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, 255, color.G, color.B, color.A);
	SDL_RenderClear(_renderer);
	SDL_SetRenderTarget(_renderer, NULL);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	return image;
}

void Graphics::SetTextureAlpha(SDL_Texture* texture, int alpha) {
	SDL_SetRenderTarget(_renderer, texture);
	if (SDL_SetRenderDrawColor(_renderer, 0, 0, 0, alpha) != 0) {
		// Handle error
		sgLogError("Error setting render draw color: %s", SDL_GetError());
	}
	SDL_RenderClear(_renderer);
	SDL_SetRenderTarget(_renderer, NULL);
}

void Graphics::ClearRenderTargetTexture(SDL_Texture* texture, Color color) {
	SDL_SetRenderTarget(_renderer, texture);
	SDL_SetRenderDrawColor(_renderer, color.R, color.G, color.B, color.A);
	SDL_RenderClear(_renderer);
	SDL_SetRenderTarget(_renderer, NULL);
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
}

void Graphics::DrawImageToImage(Image& src, Image& dst, RectangleF* srcR, RectangleF* dstR) {
	SDL_SetRenderTarget(_renderer, dst._image);
	SDL_RenderTexture(_renderer, src._image, (SDL_FRect*)srcR, (SDL_FRect*)dstR);
	SDL_SetRenderTarget(_renderer, NULL);
}

SDL_Texture* Graphics::CreateTextureFromSurface(SDL_Surface* surface) {
	SDL_Texture* t = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_SetTextureBlendMode(t, SDL_BLENDMODE_BLEND);
	if (t == NULL) {
		sgLogError("Could not create texture, Error: %s", SDL_GetError());
		return NULL;
	}
	SDL_DestroySurface(surface);
	return t;
}
