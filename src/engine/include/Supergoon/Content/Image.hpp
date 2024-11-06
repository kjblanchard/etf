#pragma once
#include <Supergoon/Content/Content.hpp>
#include <Supergoon/Primitives/Color.hpp>
#include <Supergoon/Primitives/Rectangle.hpp>
#include <string>
typedef struct SDL_Surface SDL_Surface;
typedef struct SDL_Texture SDL_Texture;
namespace Supergoon {
class Graphics;
class Image : public Content {
	enum class ImageType {
		Default,
		Surface,
		RenderTarget,
	};

   public:
	Image(std::string name);
	Image(std::string name, SDL_Surface* surface);
	Image(std::string name, int width, int height);
	~Image();
	void Draw(RectangleF& src, RectangleF& dst);
	virtual void Load() override;
	virtual void Unload() override;
	virtual const std::string Filepath() override;
	inline virtual std::string Type() override { return "Image"; }
	inline int Width() { return _width; }
	inline int Height() { return _height; }
	void Clear();
	// Updates the color modifier on this texture.
	void SetImageColor(Color colorToUse);
	void DrawImageToImage(Image& src, RectangleF& srcRect, RectangleF& dstRect);
	void SetAlpha(int alpha);

   private:
	SDL_Texture* _image;
	ImageType _imageType;
	float _width;
	float _height;
	SDL_Surface* _surface = nullptr;
	Color _imageColor = {255, 255, 255, 255};

	friend class Graphics;
};
}  // namespace Supergoon
