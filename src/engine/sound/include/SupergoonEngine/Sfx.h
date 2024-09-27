#pragma once
#include <SDL3/SDL_audio.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef struct SDL_AudioStream SDL_AudioStream;
typedef struct OggVorbis_File OggVorbis_File;
typedef struct vorbis_info vorbis_info;
typedef struct sgStream sgStream;

	typedef struct sgSfx {
	char* Filename;
	short* Buffer;
	int BufferLength;
	//  between 0 and 1.0f;
	float Volume;
	SDL_AudioSpec spec;
	// OggVorbis_File* VorbisFile;
	// vorbis_info* VorbisInfo;
	// SDL_AudioStream* Stream;

} sgSfx;

// Creates a SFX, still need to set filename
sgSfx* sgSfxNew(void);
// Load all data from the sfx to the buffer for playing.
void sgSfxLoad(sgSfx*);
// Put data from the buffer into the stream
void sgSfxPlay(sgSfx*, sgStream*);
void sgSfxPlayOneShot(const char* filename, sgStream*);
// Frees a sfx
void sgSfxDelete(sgSfx*);
// Volume should be between 0 and 1.0
// void sgSfxUpdateVolume(sgSfx*, float);

#ifdef __cplusplus
}
#endif
