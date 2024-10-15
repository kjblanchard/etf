#include <Supergoon/Log.hpp>
#include <SDL3/SDL.h>
// #include <GoonEngine/debug.h>
// #include <GoonEngine/utils.h>

#include <Supergoon/Aseprite/AsepriteAnimation.hpp>
#include <Supergoon/Aseprite/AsepriteDocument.hpp>

// #include <BbAdventures/aseprite/AsepriteAnimation.hpp>
// #include <BbAdventures/aseprite/AsepriteDocument.hpp>
using namespace Supergoon;

std::unordered_map<std::string, std::shared_ptr<AsepriteDocument>> AsepriteAnimation::_asepriteDocuments;
// #ifdef GN_PLATFORM_MACOS
// static const char *_animationPrefix = "../Resources/assets/img/";
// #else
// static const char *_animationPrefix = "assets/img/";
// #endif

AsepriteAnimation::AsepriteAnimation(std::string n) : _animNum(0), _frame(0), _nextFrame(0), _frameTime(0), _aseDocument(nullptr) {
	_rect = std::make_unique<Rectangle>();
	_filePath = n;
	Load();
}

AsepriteAnimation::~AsepriteAnimation() {
}
std::string AsepriteAnimation::Filename() {
	// auto f = _animationPrefix+ _aseDocument->meta.image;
	// char buf[1000];

	char *fullPath = NULL;
	SDL_asprintf(&fullPath, "%sassets/img/%s", SDL_GetBasePath(), _aseDocument->meta.image.c_str());
	// geGetLoadFilename(buf, sizeof(buf), f.c_str());
	return fullPath;
}

void AsepriteAnimation::Load() {
	auto iter = _asepriteDocuments.find(_filePath);
	if (iter != _asepriteDocuments.end()) {
		_aseDocument = iter->second;
	}
	_asepriteDocuments[_filePath] = std::make_shared<AsepriteDocument>(_filePath);
	_aseDocument = _asepriteDocuments[_filePath];
	UpdateRect();
}

void AsepriteAnimation::UpdateAnimation(double d) {
	_frameTime += d;
	auto frameData = _aseDocument->frames[_frame];
	auto animData = _aseDocument->meta.frameTags[_animNum];
	while (_frameTime >= frameData.duration) {
		_frameTime -= frameData.duration;
		if (animData.direction == "pingpong") {
			if (_reverse) {
				_nextFrame = _frame - 1;
				if (_nextFrame < animData.from) {
					_nextFrame = animData.from + 1;
					_reverse = false;
				}
			} else {
				_nextFrame = _frame + 1;
				if (_nextFrame > animData.to) {
					_nextFrame = animData.to - 1;
					_reverse = true;
				}
			}
		} else {
			if (animData.direction == "forward") {
				_nextFrame = (_frame + 1 > animData.to) ? animData.from : _frame + 1;
			} else if (animData.direction == "reverse") {
				_nextFrame = (_frame - 1 < animData.from) ? animData.to : _frame - 1;
			}
		}
		_frame = _nextFrame;
		frameData = _aseDocument->frames[_frame];
		UpdateRect();
	}
}

void AsepriteAnimation::PlayAnimation(std::string a) {
	for (size_t i = 0; i < _aseDocument->meta.frameTags.size(); i++) {
		auto& anim = _aseDocument->meta.frameTags[i];
		if (anim.name == a) {
			_animNum = i;
			_frame = anim.from;
			_frameTime = 0;
			UpdateRect();
			return;
		}
	}
	sgLogWarn("Could not find animation for %s", a.c_str());
}
void AsepriteAnimation::UpdateRect() {
	_rect->X = _aseDocument->frames[_frame].frame.x;
	_rect->Y = _aseDocument->frames[_frame].frame.y;
	_rect->W = _aseDocument->frames[_frame].frame.w;
	_rect->H = _aseDocument->frames[_frame].frame.h;
}

Rectangle& AsepriteAnimation::FrameCoords() {
	return *(_rect.get());
}
