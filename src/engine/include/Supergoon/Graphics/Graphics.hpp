#pragma once
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

#include <Supergoon/Color.hpp>
#include <Supergoon/Rectangle.hpp>
#include <string>
namespace Supergoon {

class Image;

class Graphics {
   public:
	inline Graphics() { _instance = this; }
	void CreateWindow(int width, int height, std::string name);
	void InitializeImGui();
	void DrawStart();
	void DrawEnd();
	void DrawImGui();
	void DrawImage(Image& image, RectangleF* srcR, RectangleF* dstR);

	void DrawImageToImage(Image& src, Image& dst, RectangleF* srcR, RectangleF* dstR);
	SDL_Texture* CreateTextureFromSurface(SDL_Surface* surface);
	SDL_Texture* CreateRenderTargetTexture(int width, int height, Color color = Color{0, 0, 0, 255});
	void ClearRenderTargetTexture(SDL_Texture* texture, Color color = Color{0, 0, 0, 255});
	void SetTextureAlpha(SDL_Texture* texture, int alpha);

	inline static Graphics& Instance() { return *_instance; }

   private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	static Graphics* _instance;
};
}  // namespace Supergoon
