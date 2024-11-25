#pragma once
#include <Supergoon/Primitives/Rectangle.hpp>
#include <memory>
#include <string>
#include <unordered_map>
namespace Supergoon {
class AsepriteDocument;
class AsepriteAnimation {
   public:
	AsepriteAnimation(std::string n);
	std::string Filename();
	void Load();
	void UpdateAnimation(double d);
	void PlayAnimation(std::string a);
	RectangleF& FrameCoords();

   private:
	void UpdateRect();
	std::string _filePath;
	int _animNum;
	int _frame;
	int _nextFrame;
	float _frameTime;
	bool _reverse = false;	// pingpong
	RectangleF _rect;
	std::shared_ptr<AsepriteDocument> _aseDocument;
};
}  // namespace Supergoon
