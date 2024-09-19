#pragma once

#ifdef __cplusplus
extern "C" {
#endif
typedef SDL_AudioStream SDL_AudioStream;

typedef struct {
	const char* Filename;
	uint64_t LoopStart;
	uint64_t LoopEnd;
	uint64_t CurrentLoopBytesRead;
	int SampleRate;
	SDL_AudioStream* Stream;
	short *Buffer;
	int Channels;

} sgBgm;

void sgBgmInit(sgBgm* bgm);
void sgBgmUpdate(sgBgm* bgm);
void sgBgmDestroy(sgBgm* bgm);

#ifdef __cplusplus
}
#endif
