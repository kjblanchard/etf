#pragma once
#include <Supergoon/Rectangle.hpp>
#include <memory>
#include <string>
#include <unordered_map>
namespace Supergoon {
class AsepriteDocument;
class AsepriteAnimation {
   public:
	AsepriteAnimation(std::string n);
	~AsepriteAnimation();
	std::string Filename();
	void Load();
	void UpdateAnimation(double d);
	void PlayAnimation(std::string a);
	Rectangle& FrameCoords();

   private:
	void UpdateRect();
	std::string _filePath;
	int _animNum;
	int _frame;
	int _nextFrame;
	float _frameTime;
	bool _reverse = false;	// pingpong
	std::unique_ptr<Rectangle> _rect;
	std::shared_ptr<AsepriteDocument> _aseDocument;
	static std::unordered_map<std::string, std::shared_ptr<AsepriteDocument>> _asepriteDocuments;
};
}  // namespace Supergoon
