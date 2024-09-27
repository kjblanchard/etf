#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>
#include <SupergoonEngine/Stream.h>

#include <Supergoon/Sound.hpp>
#include <SupergoonEngine/Log.hpp>

namespace Supergoon {
Sound::Sound() {
	puts("What");
}
void Sound::InitializeSound() {
	for (size_t i = 0; i < _totalSfxStreams; i++) {
		auto stream = sgStreamNew();
		_sfxStreams.push_back(std::unique_ptr<sgStream>(stream));
		_usableSfxStreams.push(stream);
	}
}
bool Sound::LoadBgm(std::string filename, float volume, int loops) {
	if (_bgm) {
		sgBgmDelete(_bgm);
		_bgm = nullptr;
	}
	if (volume < 0 || volume > 1.0) {
		SDL_LogWarn(SG_LOG_LEVEL_sound, "Volume passed in for %s is %f which is below 0 or greater than 1, setting to 1\n", filename.c_str(), volume);
		volume = 1.0;
	}
	char *fullPath = NULL;
	SDL_asprintf(&fullPath, "%sassets/%s%s", SDL_GetBasePath(), filename.c_str(), ".ogg");
	auto bgm = sgBgmNew();
	bgm->Filename = fullPath;
	bgm->Loops = loops;
	bgm->Volume = volume * _globalBgmVolume;
	sgBgmLoad(bgm);
	_bgm = bgm;
	_playingBgmVolume = volume;
	return true;
}
void Sound::PlayBgm() {
	if (!_bgm || !_bgm->CanPlay) {
		return;
	}
	sgBgmPlay(_bgm);
}

void Sound::Update() {
	if (_bgm) {
		sgBgmUpdate(_bgm);
	}
}
void Sound::UpdatePlayingBgmVolume() {
	if (!_bgm) {
		return;
	}
	sgBgmUpdateVolume(_bgm, _globalBgmVolume * _playingBgmVolume);
}
void Sound::SetGlobalBgmVolume(float volume) {
	if (volume < 0 || volume > 1.0) {
		return;
	}
	_globalBgmVolume = volume;
	UpdatePlayingBgmVolume();
}

void Sound::SetPlayingBgmVolume(float volume) {
	if (volume < 0 || volume > 1.0) {
		return;
	}
	_playingBgmVolume = volume;
	UpdatePlayingBgmVolume();
}

void Sound::LoadSfx(std::string, float) {
}
void Sound::PlaySfx(std::string filename, float) {
	if (_usableSfxStreams.empty()) {
		puts("Empty");
		for (auto it = _playingStreams.begin(); it != _playingStreams.end();) {
			if (sgStreamIsFinished(*it)) {
				_usableSfxStreams.push(*it);
				it = _playingStreams.erase(it);
			} else {
				++it;
			}
		}
		if (_usableSfxStreams.empty()) {
			return;
		}
	}
	auto stream = _usableSfxStreams.front();
	auto fullFilename = SDL_GetBasePath() + std::string("assets/") + filename + ".ogg";
	// SDL_asprintf(&fullPath, "%sassets/%s%s", SDL_GetBasePath(), filename.c_str(), ".ogg");
	sgSfxPlayOneShot(fullFilename.c_str(), stream);
	_playingStreams.push_back(stream);
	_usableSfxStreams.pop();
}

}  // namespace Supergoon
