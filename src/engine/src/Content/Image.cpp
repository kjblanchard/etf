#include <SDL3/SDL.h>
#include <SupergoonEngine/log.h>
#define STB_IMAGE_IMPLEMENTATION
#include <SupergoonEngine/nothings/stb_image.h>

#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
using namespace Supergoon;

SDL_Surface *loadPNG(const char *filename, void** dataToFree) {
	// Read data
	int width, height, bytesPerPixel;
	void *data = stbi_load(filename, &width, &height, &bytesPerPixel, 0);

	// Calculate pitch
	int pitch;
	pitch = width * bytesPerPixel;
	pitch = (pitch + 3) & ~3;

	// Setup relevance bitmask
	int Rmask, Gmask, Bmask, Amask;
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
	Rmask = 0x000000FF;
	Gmask = 0x0000FF00;
	Bmask = 0x00FF0000;
	Amask = (bytesPerPixel == 4) ? 0xFF000000 : 0;
#else
	int s = (bytesPerPixel == 4) ? 0 : 8;
	Rmask = 0xFF000000 >> s;
	Gmask = 0x00FF0000 >> s;
	Bmask = 0x0000FF00 >> s;
	Amask = 0x000000FF >> s;
#endif
	SDL_Surface *surface = SDL_CreateSurfaceFrom(width, height, SDL_GetPixelFormatForMasks(bytesPerPixel * 8, Rmask, Gmask, Bmask, Amask), data, pitch);
	if (!surface) {
		stbi_image_free(data);
		// NOTE: Should free stbi_load 'data' variable here
		return NULL;
	}
	*dataToFree = data;
	// stbi_image_free(data);
	return surface;
}

Image::Image(std::string name) : Content(name) {
	_imageType = ImageType::Default;
}

Image::Image(std::string name, SDL_Surface *surface) : Content(name), _surface(surface) {
	_imageType = ImageType::Surface;
}

Image::Image(std::string name, int width, int height) : Content(name), _width(width), _height(height) {
	_imageType = ImageType::RenderTarget;
}
void Image::Load() {
	auto graphics = Graphics::Instance();
	switch (_imageType) {
		case ImageType::Default: {
			void* pngData = nullptr;
			SDL_Surface *s = loadPNG(_filePath.c_str(), &pngData);
			if (!s) {
				sgLogError("Could not load PNG properly, content not fully loaded");
				_image = nullptr;
				return;
			}
			_image = graphics->CreateTextureFromSurface(s);
			SDL_free(pngData);
			break;
		}
		case ImageType::Surface: {
			_image = graphics->CreateTextureFromSurface(_surface);
			break;
		}
		case ImageType::RenderTarget: {
			_image = graphics->CreateRenderTargetTexture(_width, _height);
			break;
		}
	}
	SDL_GetTextureSize(_image, &_width, &_height);
}
void Image::Unload() {
	if (_image) {
		SDL_DestroyTexture(_image);
		_image = nullptr;
	}
	if (_surface) {
		SDL_DestroySurface(_surface);
		_surface = nullptr;
	}
}
const std::string Image::Filepath() {
	return _filePath;
}

void Image::SetAlpha(int alpha) {
	auto graphics = Graphics::Instance();
	graphics->SetTextureAlpha(_image, alpha);
}

Image::~Image() {
	Unload();
}

void Image::DrawImageToImage(Image &src, RectangleF &srcRect, RectangleF &dstRect) {
	auto graphics = Graphics::Instance();
	graphics->DrawImageToImage(src, *this, &srcRect, &dstRect);
}

void Image::Draw(RectangleF &src, RectangleF &dst) {
	auto graphics = Graphics::Instance();
	graphics->DrawImage(*this, &src, &dst);
}
