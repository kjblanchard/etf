#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_log.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>
#include <SupergoonEngine/Stream.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Sound.hpp>
#include <SupergoonEngine/Log.hpp>

namespace Supergoon {
void Sound::InitializeSound() {
	for (size_t i = 0; i < _totalSfxStreams; i++) {
		auto stream = sgStreamNew();
		_sfxStreams.push_back(std::unique_ptr<sgStream>(stream));
		_usableSfxStreams.push(stream);
	}
}

bool Sound::LoadBgm(std::string filename, float volume, int loops) {
	char* fullPath = NULL;
	SDL_asprintf(&fullPath, "%sassets/bgm/%s%s", SDL_GetBasePath(), filename.c_str(), ".ogg");
	if (_bgm && !strcmp(_bgm->Filename, fullPath)) {
		SDL_free(fullPath);
		return false;
	}
	if (_bgm) {
		sgBgmDelete(_bgm);
		_bgm = nullptr;
	}
	if (volume < 0 || volume > 1.0) {
		SDL_LogWarn(SG_LOG_LEVEL_sound, "Volume passed in for %s is %f which is below 0 or greater than 1, setting to 1\n", filename.c_str(), volume);
		volume = 1.0;
	}
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

void Sound::PauseBgm() {
	if (!_bgm || !_bgm->IsPlaying) {
		return;
	}
	sgBgmStop(_bgm);
}

void Sound::Update() {
	if (_bgm) {
		sgBgmUpdate(_bgm);
	}
	if (_usableSfxStreams.empty()) {
		CheckForStaleSfxStreams();
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

void Sound::CheckForStaleSfxStreams() {
	for (auto it = _playingStreams.begin(); it != _playingStreams.end();) {
		if (sgStreamIsFinished(*it)) {
			_usableSfxStreams.push(*it);
			it = _playingStreams.erase(it);
		} else {
			++it;
		}
	}
}

void Sound::PlaySfx(Sfx* sfx, float) {
	if (_usableSfxStreams.empty()) {
		SDL_LogWarn(SG_LOG_LEVEL_sound, "No SFX buffers available to play sound %s\n", sfx->Filepath().c_str());
		return;
	}
	auto stream = _usableSfxStreams.front();
	sgSfxPlay(sfx->SgSfx(), stream);
	_playingStreams.push_back(stream);
	_usableSfxStreams.pop();
}

}  // namespace Supergoon
