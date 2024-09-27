#pragma once
#include <SupergoonEngine/Stream.h>

#include <queue>
#include <string>
typedef struct sgBgm sgBgm;
typedef struct sgSfx sgSfx;
namespace Supergoon {
class Sound {
   public:
	Sound();
	void InitializeSound();
	/**
	 * @brief
	 * @param filename name of the file to load, assumes .ogg
	 * @param volume 0 - 1.0
	 * @param loops -1 is infinite, else number of loops
	 * @return true loaded successfully
	 * @return false error
	 */
	bool LoadBgm(std::string filename, float volume = 1.0, int loops = -1);
	// Plays a bgm loaded into the bgm slot
	void PlayBgm();
	//  Sets current playing bgm volume, 0 - 1.0f
	void SetPlayingBgmVolume(float volume);
	//  Sets the global volume multiplier for bgm, 0 - 1.0f
	void SetGlobalBgmVolume(float volume);

	//  Preloads a sfx, useful if you want to load the sound previously, but not used currently as all sounds are just batched.
	void LoadSfx(std::string filename, float volume = 1.0);
	void PlaySfx(std::string filename, float volume = 1.0);

   private:
	//  Updates all internal bgms
	void Update();
	void UpdatePlayingBgmVolume();
	const int _totalSfxStreams = 4;
	float _globalBgmVolume = 1.0f;
	float _playingBgmVolume = 0;
	std::vector<std::unique_ptr<sgStream>> _sfxStreams;
	std::vector<sgStream*> _playingStreams;
	std::queue<sgStream*> _usableSfxStreams;
	sgBgm* _bgm;
	friend class Game;
};
}  // namespace Supergoon