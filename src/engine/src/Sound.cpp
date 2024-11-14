#include <SDL3/SDL_filesystem.h>
#include <SupergoonEngine/Bgm.h>
#include <SupergoonEngine/Sfx.h>
#include <SupergoonEngine/Stream.h>
#include <SupergoonEngine/log.h>

#include <Supergoon/Content/ContentRegistry.hpp>
#include <Supergoon/Events.hpp>
#include <Supergoon/Sound.hpp>
#include <Supergoon/Tween/Tween.hpp>
#include <cassert>

namespace Supergoon {
Sound* Sound::_instance = nullptr;
void Sound::InitializeSound() {
	Events::RegisterEventHandler(Events::BuiltinEvents.PlayBgmEvent, [this](int slot, void* name, void*) {
		auto nameStr = std::string((const char*)name);
		// TODO this doesn't pass in any thing correctly, probably pass in an actual object
		LoadBgm(nameStr, 1, -1, slot);
		PlayBgm(slot);
		SDL_free(name);
	});
	Events::RegisterEventHandler(Events::BuiltinEvents.StopBgmEvent, [this](int slot, void* shouldFade, void*) {
		if (shouldFade) {
			auto fade = (bool)shouldFade;
			if (fade) {
				StopBgmFadeout(slot, 0.25);
				return;
			}
		}
		StopBgm(slot);
	});
	for (size_t i = 0; i < _totalSfxStreams; i++) {
		auto stream = sgStreamNew();
		_sfxStreams.push_back(std::unique_ptr<sgStream>(stream));
		_usableSfxStreams.push(stream);
	}
	_bgms.resize(_bgmSlots);
	_tweens.resize(_bgmSlots);
}

bool Sound::LoadBgm(std::string filename, float volume, int loops, int slot) {
	char* fullPath = NULL;
	auto& _bgm = _bgms[slot];
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
		sgLogWarn("Volume passed in for %s is %f which is below 0 or greater than 1, setting to 1\n", filename.c_str(), volume);
		volume = 1.0;
	}
	auto bgm = sgBgmNew();
	bgm->Filename = fullPath;
	bgm->Loops = loops;
	bgm->Volume = volume * _globalBgmVolume;
	sgBgmLoad(bgm);
	_bgm = bgm;
	_bgmOriginalVolume = _playingBgmVolume = volume;
	return true;
}

void Sound::PlayBgm(int slot) {
	auto& _bgm = _bgms[slot];
	if (!_bgm || !_bgm->CanPlay) {
		return;
	}
	SetPlayingBgmVolume(_bgmOriginalVolume);
	sgBgmPlay(_bgm);
}

void Sound::PauseBgm(int slot) {
	auto& _bgm = _bgms[slot];
	if (!_bgm || !_bgm->IsPlaying) {
		return;
	}
	sgBgmPause(_bgm);
}

void Sound::StopBgm(int slot) {
	auto& _bgm = _bgms[slot];
	if (!_bgm) {
		return;
	}
	sgBgmStop(_bgm);
}

void Sound::StopBgmFadeout(int slot, float fadeTime) {
	auto& _bgm = _bgms[slot];
	if (!_bgm || !_bgm->IsPlaying || _fadingOut) {
		return;
	}
	if (_tweens[slot]) {
		SDL_free(_tweens[slot]);
		_tweens[slot] = nullptr;
	}
	_tweens[slot] = new Tween(_playingBgmVolume, 0, fadeTime, &_playingBgmVolume, Supergoon::Easings::Linear);
	_tweens[slot]->UpdateFunc = [this]() {
		UpdatePlayingBgmVolume();
	};
	_fadingOut = true;
}

void Sound::Update() {
	for (auto bgm : _bgms) {
		if (bgm) {
			sgBgmUpdate(bgm);
		}
	}

	if (_usableSfxStreams.empty()) {
		CheckForStaleSfxStreams();
	}
	for (size_t i = 0; i < _tweens.size(); i++) {
		auto& tween = _tweens[i];
		if (!tween) return;
		if (!tween->Complete()) {
			tween->Update();
			// UpdatePlayingBgmVolume();
			// SetPlayingBgmVolume(_playingBgmVolume);
		} else {
			StopBgm();
			SDL_free(_tweens[i]);
			tween = nullptr;
			_fadingOut = false;
		}
	}
}

void Sound::UpdatePlayingBgmVolume(int slot) {
	auto& _bgm = _bgms[slot];
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

void Sound::PlaySfx(Sfx* sfx, float volume) {
	if (_usableSfxStreams.empty()) {
		sgLogWarn("No SFX buffers available to play sound %s\n", sfx->Filepath().c_str());
		return;
	}
	auto stream = _usableSfxStreams.front();
	sfx->SgSfx()->Volume = volume * _globalSfxVolume;
	sgSfxPlay(sfx->SgSfx(), stream);
	_playingStreams.push_back(stream);
	_usableSfxStreams.pop();
}

}  // namespace Supergoon
