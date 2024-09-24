#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SupergoonEngine/Bgm.h>

#include <Supergoon/Bgm.hpp>
#include <SupergoonEngine/Log.hpp>

namespace Supergoon {
bool Bgm::LoadBgm(std::string filename, float volume, int loops) {
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
void Bgm::PlayBgm() {
	if (!_bgm || !_bgm->CanPlay) {
		return;
	}
	sgBgmPlay(_bgm);
}

void Bgm::Update() {
	if (_bgm) {
		sgBgmUpdate(_bgm);
	}
}
void Bgm::UpdatePlayingBgmVolume() {
	if (!_bgm) {
		return;
	}
	sgBgmUpdateVolume(_bgm, _globalBgmVolume * _playingBgmVolume);
}
void Bgm::SetGlobalBgmVolume(float volume) {
	if (volume < 0 || volume > 1.0) {
		return;
	}
	_globalBgmVolume = volume;
	UpdatePlayingBgmVolume();
}

void Bgm::SetPlayingBgmVolume(float volume) {
	if (volume < 0 || volume > 1.0) {
		return;
	}
	_playingBgmVolume = volume;
	UpdatePlayingBgmVolume();
}

}  // namespace Supergoon
