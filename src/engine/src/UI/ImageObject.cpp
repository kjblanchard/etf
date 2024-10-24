#include <SDL3/SDL.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/UI/ImageObject.hpp>
#include <Supergoon/UI/Panel.hpp>
#include <SupergoonEngine/nlohmann/json.hpp>

using namespace Supergoon;
using json = nlohmann::json;

void ImageObject::OnDirty() {
	auto parentBoundsX = Parent ? Parent->Bounds.X : 0;
	auto parentBoundsY = Parent ? Parent->Bounds.Y : 0;
	Bounds.X = Offset.X + parentBoundsX;
	Bounds.Y = Offset.Y + parentBoundsY;
	ImagePtr->SetAlpha(Transparency);
}

ImageObject::ImageObject(Panel* parent, json& imageJson) : UIObject(parent) {
	WidgetType = (int)BuiltinWidgetTypes::Image;
	Visible = imageJson["visible"].get<bool>();
	Transparency = imageJson["alpha"].get<float>();
	auto name = imageJson["name"].get<std::string>();
	auto sourceData = imageJson["source"];
	auto destinationData = imageJson["destination"];
	auto imageName = sourceData["name"].get<std::string>();
	auto fullPath = std::string(SDL_GetBasePath()) + "assets/img/" + imageName + ".png";
	ImagePtr = ContentRegistry::CreateContent<Image>(fullPath);
	Offset.X = destinationData["x"].get<float>();
	Offset.Y = destinationData["y"].get<float>();
	auto x = sourceData["x"].get<float>();
	auto y = sourceData["y"].get<float>();
	auto w = sourceData["width"].get<float>();
	auto h = sourceData["height"].get<float>();
	// Set the source rect, if it is all 0 use the full image, but it must be loaded to get size
	if (w == 0 && h == 0) {
		ImagePtr->Load();
		w = ImagePtr->Width();
		h = ImagePtr->Height();
	}
	ImageSourceRect = RectangleF{x, y, w, h};
	Bounds.W = destinationData["width"].get<float>();
	Bounds.H = destinationData["height"].get<float>();
};

void ImageObject::Draw() {
	ImagePtr->Draw(ImageSourceRect, Bounds);
}
