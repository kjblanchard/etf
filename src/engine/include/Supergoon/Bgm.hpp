#pragma once
#include <string>
typedef struct sgBgm sgBgm;
namespace Supergoon {
class Bgm {
   public:
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

   private:
	//  Updates all internal bgms
	void Update();
	void UpdatePlayingBgmVolume();
	float _globalBgmVolume = 1.0f;
	float _playingBgmVolume = 0;
	sgBgm* _bgm;
	friend class Game;
};
}  // namespace Supergoon