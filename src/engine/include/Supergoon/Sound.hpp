#pragma once
#include <SupergoonEngine/Stream.h>

#include <Supergoon/Content/Sfx.hpp>
#include <memory>
#include <queue>
#include <string>
typedef struct sgBgm sgBgm;
typedef struct sgSfx sgSfx;
namespace Supergoon {
class Tween;
class Sound {
   public:
	inline Sound() { _instance = this; }
	//  Initializes the Supergoon Sound
	void InitializeSound();
	/**
	 * @brief Loads a bgm into the main BGM slot of sound
	 * @param filename name of the file to load, assumes .ogg
	 * @param volume 0 - 1.0
	 * @param loops -1 is infinite, else number of loops
	 * @return true loaded successfully
	 * @return false error
	 */
	bool LoadBgm(std::string filename, float volume = 1.0, int loops = -1);
	// Plays a bgm loaded into the bgm slot
	void PlayBgm();
	// Pauses bgm in bgm slot
	void PauseBgm();
	// Pauses bgm in bgm slot
	void StopBgm();
	void StopBgmFadeout();
	//  Sets current playing bgm volume, 0 - 1.0f
	void SetPlayingBgmVolume(float volume);
	//  Sets the global volume multiplier for bgm, 0 - 1.0f
	void SetGlobalBgmVolume(float volume);
	inline void SetGlobalSfxVolume(float volume) { _globalSfxVolume = volume; };
	void PlaySfx(Sfx* sfx, float volume = 1.0);
	static inline Sound* Instance() { return _instance; }

   private:
	//  Updates all internal bgms
	void Update();
	void CheckForStaleSfxStreams();
	void UpdatePlayingBgmVolume();
	const size_t _totalSfxStreams = 4;
	float _globalBgmVolume = 1.0f;
	float _globalSfxVolume = 1.0f;
	float _playingBgmVolume = 0;
	std::vector<std::unique_ptr<sgStream>> _sfxStreams;
	std::vector<sgStream*> _playingStreams;
	std::queue<sgStream*> _usableSfxStreams;
	float _bgmOriginalVolume = 0;
	bool _fadingOut = false;
	sgBgm* _bgm = nullptr;
	Tween* _bgmTween = nullptr;
	static Sound* _instance;
	friend class Game;
};
}  // namespace Supergoon
