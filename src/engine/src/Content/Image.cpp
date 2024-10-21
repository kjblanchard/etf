#include <SDL3/SDL.h>
#include <SupergoonEngine/log.h>
#include <png.h>

#include <Supergoon/Content/Image.hpp>
#include <Supergoon/Graphics/Graphics.hpp>
using namespace Supergoon;
static void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	Uint8 *target_pixel = (Uint8 *)surface->pixels + y * surface->pitch + x * sizeof(Uint32);
	*(Uint32 *)target_pixel = pixel;
}

static SDL_Surface *loadPNG(const char *filename) {
	FILE *file = fopen(filename, "rb");
	if (!file) {
		fprintf(stderr, "Error: Unable to open file %s\n", filename);
		return NULL;
	}

	// Check if the file is a PNG file
	unsigned char header[8];
	fread(header, 1, 8, file);
	if (png_sig_cmp(header, 0, 8)) {
		fclose(file);
		fprintf(stderr, "Error: %s is not a PNG file\n", filename);
		return NULL;
	}

	// Initialize libpng structures
	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		fclose(file);
		fprintf(stderr, "Error: Unable to create png_struct\n");
		return NULL;
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		fclose(file);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		fprintf(stderr, "Error: Unable to create png_info\n");
		return NULL;
	}

	// Set error handling
	if (setjmp(png_jmpbuf(png_ptr))) {
		fclose(file);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fprintf(stderr, "Error: Error during png read\n");
		return NULL;
	}

	// Initialize PNG IO
	png_init_io(png_ptr, file);
	png_set_sig_bytes(png_ptr, 8);

	// Read PNG info
	png_read_info(png_ptr, info_ptr);
	int width = png_get_image_width(png_ptr, info_ptr);
	int height = png_get_image_height(png_ptr, info_ptr);
	png_byte color_type = png_get_color_type(png_ptr, info_ptr);
	png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

	// Read any color_type to 8-bit depth, RGBA format.
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_palette_to_rgb(png_ptr);

	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand_gray_1_2_4_to_8(png_ptr);

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_tRNS_to_alpha(png_ptr);

	if (color_type == PNG_COLOR_TYPE_RGB ||
		color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);

	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);

	// Update the png info struct.
	png_read_update_info(png_ptr, info_ptr);

	// Allocate memory for image data
	png_bytep *row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++) {
		row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png_ptr, info_ptr));
	}

	// Read the image data
	png_read_image(png_ptr, row_pointers);

	// Create an SDL surface with the correct format
	SDL_Surface *surface = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA8888);
	// SDL_Surface *surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA8888);
	if (!surface) {
		fprintf(stderr, "Error: Unable to create SDL surface\n");
		for (int y = 0; y < height; y++) {
			free(row_pointers[y]);
		}
		free(row_pointers);
		fclose(file);
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return NULL;
	}

	// Copy pixel data from PNG to SDL surface
	for (int y = 0; y < height; y++) {
		png_bytep row = row_pointers[y];
		for (int x = 0; x < width; x++) {
			png_bytep px = &(row[x * 4]);
			// Uint32 color = SDL_MapRGBA(surface->format, px[0], px[1], px[2], px[3]);
			Uint32 color = SDL_MapSurfaceRGBA(surface, px[0], px[1], px[2], px[3]);
			putpixel(surface, x, y, color);
		}
	}

	// Free libpng allocated memory
	for (int y = 0; y < height; y++) {
		free(row_pointers[y]);
	}
	free(row_pointers);

	// Clean up libpng structures
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(file);

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
			SDL_Surface *s = loadPNG(_filePath.c_str());
			if (!s) {
				sgLogError("Could not load PNG properly, content not fully loaded");
				_image = nullptr;
				return;
			}
			_image = graphics.CreateTextureFromSurface(s);
			break;
		}
		case ImageType::Surface: {
			_image = graphics.CreateTextureFromSurface(_surface);
			break;
		}
		case ImageType::RenderTarget: {
			_image = graphics.CreateRenderTargetTexture(_width, _height);
			break;
		}
	}
	SDL_GetTextureSize(_image, &_width, &_height);
}
void Image::Unload() {
}
const std::string Image::Filepath() {
	return _filePath;
}

void Image::SetAlpha(int alpha) {
	auto graphics = Graphics::Instance();
	graphics.SetTextureAlpha(_image, alpha);
}

Image::~Image() {
	SDL_DestroyTexture(_image);
}

void Image::DrawImageToImage(Image &src, RectangleF &srcRect, RectangleF &dstRect) {
	auto graphics = Graphics::Instance();
	graphics.DrawImageToImage(src, *this, &srcRect, &dstRect);
}

void Image::Draw(RectangleF &src, RectangleF &dst) {
	auto graphics = Graphics::Instance();
	graphics.DrawImage(*this, &src, &dst);
}
