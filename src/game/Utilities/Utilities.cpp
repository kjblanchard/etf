#include <SDL3/SDL_filesystem.h>

#include <Supergoon/Supergoon.hpp>
#include <Utilities/Utilities.hpp>
#include <string>

using namespace Supergoon;
// 200/70 for textbox
Panel* Supergoon::CreateUITextbox(std::string name, Point screenLoc, Point size, bool screen) {
	auto ui = UI::UIInstance.get();
	auto textPanel = new Panel(ui, "textTesting" + name);
	textPanel->Offset = {(float)screenLoc.X, (float)screenLoc.Y};
	auto text = new UIText(textPanel, "Hello world!", "textman" + name);
	text->Offset = {8, 8};
	// Test creating the uitextbox
	// First, lets load in the picture for uiimage so that we can draw from it to the new one
	auto path = std::string(SDL_GetBasePath()) + "assets/img/uibase.png";
	auto uiImageFull = ContentRegistry::CreateContent<Image>(path);
	uiImageFull->LoadContent();
	// Create ui text image of the right size as a render target
	// float fullSizeX = 200;
	// float fullSizeY = 48;
	float fullSizeX = size.X;
	float fullSizeY = size.Y;
	text->Bounds.W = fullSizeX - (text->Offset.X * 2);
	text->Bounds.H = fullSizeY - (text->Offset.Y * 2);
	// text->CenterText = true;
	// text->WordWrap = true;
	text->SetCenter(true);
	text->SetCenterY(true);
	text->SetWordWrap(true);
	// text->SetCenter(true);
	auto textBoxImage = ContentRegistry::CreateContent<Image, int, int>("uitextbox" + name, (int)fullSizeX, (int)fullSizeY);
	textBoxImage->LoadContent();
	// Set the background
	textBoxImage->Clear({80, 0, 80, 220});
	float sizeX = 8;
	float sizeY = 9;
	textBoxImage->SetAlpha(200);
	// Draw the corners
	// tl
	auto srcRect = RectangleF{0, 0, sizeX, sizeY};
	auto dstRect = RectangleF{0, 0, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// tr
	srcRect = RectangleF{uiImageFull->Width() - sizeX, 0, sizeX, sizeY};
	dstRect = RectangleF{fullSizeX - sizeX, 0, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// bl
	srcRect = RectangleF{0, uiImageFull->Height() - sizeY, sizeX, sizeY};
	dstRect = RectangleF{0, fullSizeY - sizeY, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// br
	srcRect = RectangleF{uiImageFull->Width() - sizeX, uiImageFull->Height() - sizeY, sizeX, sizeY};
	dstRect = RectangleF{fullSizeX - sizeX, fullSizeY - sizeY, sizeX, sizeY};
	textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	// draw the bars
	int length = fullSizeX - (sizeX);
	int height = fullSizeY - (sizeY);
	// top
	srcRect = RectangleF{1 + sizeX, 0, 1, sizeY};
	for (auto i = sizeX; i < length; i++) {
		dstRect = RectangleF{(float)i, 0, 1, sizeY};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// bottom
	for (auto i = sizeX; i < length; i++) {
		dstRect = RectangleF{(float)i, fullSizeY - sizeY + 4, 1, sizeY};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// left
	srcRect = RectangleF({0, sizeY + 1, sizeX, 1});
	for (auto i = sizeY; i < height; i++) {
		dstRect = RectangleF{0, (float)i, sizeX, 1};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}
	// right
	for (auto i = sizeY; i < height; i++) {
		dstRect = RectangleF{fullSizeX - sizeX + 3, (float)i, sizeX, 1};
		textBoxImage->DrawImageToImage(*uiImageFull, srcRect, dstRect);
	}

	// Add this image to the ui panel
	auto textBoxUIImage = new UIImage(textPanel, "textBoxImage");
	textBoxUIImage->ImagePtr = textBoxImage;
	textBoxUIImage->SetVisible(true);
	textBoxUIImage->Bounds = RectangleF{0, 0, (float)textBoxImage->Width(), (float)textBoxImage->Height()};
	textBoxUIImage->Offset.X = 0;
	textBoxUIImage->Offset.Y = 0;
	// Setup the animators
	if (screen) {
		auto animator = std::make_shared<UIObjectAnimatorBase>("levelDisplayAnimator");
		auto waitTween = new Tween(1.0f);
		auto fadeOutTween = new Tween(255, 0, 0.5, textPanel->AlphaHandle(), Supergoon::Easings::Linear);
		fadeOutTween->EndFunc = [textPanel]() {
			textPanel->SetVisible(false);
			textPanel->SetAlpha(255);
		};
		animator->AddUIObjectTween(waitTween, textPanel);
		animator->AddUIObjectTween(fadeOutTween, textPanel);
		textPanel->Animators.push_back(animator);
	}
	return textPanel;
}
