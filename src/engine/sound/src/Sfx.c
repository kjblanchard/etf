#include <SDL3/SDL_audio.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_misc.h>
#include <SupergoonEngine/Sfx.h>
#include <vorbis/vorbisfile.h>

#include <SupergoonEngine/Log.hpp>
#define VORBIS_REQUEST_SIZE 4096  // Size of vorbis requests, usually recommend to be 4096

sgSfx* sgSfxNew(void) {
	sgSfx* sfx = SDL_malloc(sizeof(*sfx));
	sfx->Filename = NULL;
	sfx->Buffer = NULL;
	sfx->BufferLength = 0;
	sfx->Volume = 1.0f;
	sfx->VorbisFile = SDL_malloc(sizeof(*sfx->VorbisFile));
	sfx->VorbisInfo = NULL;
	sfx->Stream = NULL;
	return sfx;
}

void sgSfxLoad(sgSfx* sfx) {
	int result = ov_fopen(sfx->Filename, sfx->VorbisFile);
	if (result != 0) {
		SDL_LogError(0, "Could not open audio in %s: %d\n", sfx->Filename, result);
		return;
	}
	sfx->VorbisInfo = ov_info(sfx->VorbisFile, -1);
	sfx->BufferLength = ov_pcm_total(sfx->VorbisFile, -1) * sfx->VorbisInfo->channels * sizeof(short);
	sfx->Buffer = SDL_malloc(sfx->BufferLength);
	int totalBytesRead = 0;
	int fullyLoaded = false;
	while (!fullyLoaded) {
		int bytes_read = ov_read(sfx->VorbisFile, (char*)sfx->Buffer + totalBytesRead, VORBIS_REQUEST_SIZE, 0, sizeof(short), 1, 0);
		totalBytesRead += bytes_read;
		if (bytes_read == 0) {
			fullyLoaded = true;
		}
	}
	const SDL_AudioSpec srcspec = {SDL_AUDIO_S16LE, sfx->VorbisInfo->channels, sfx->VorbisInfo->rate};
	sfx->Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &srcspec, NULL, NULL);
	SDL_SetAudioStreamGain(sfx->Stream, sfx->Volume);
}

void sgSfxPlay(sgSfx* sfx) {
	if (!sfx->Stream || !sfx->Buffer) {
		SDL_LogWarn(SG_LOG_LEVEL_sound, "Trying to play a sfx without setting a stream or loading properly: %s", sfx->Filename);
		return;
	}
	bool result = SDL_PutAudioStreamData(sfx->Stream, sfx->Buffer, sfx->BufferLength);
	if (!result) {
		SDL_LogWarn(SG_LOG_LEVEL_sound, "Could not put data into stream for %s: %s", sfx->Filename, SDL_GetError());
	}
	SDL_ResumeAudioStreamDevice(sfx->Stream);
}

void sgSfxDelete(sgSfx* sfx) {
	SDL_free(sfx->Filename);
	SDL_free(sfx->Buffer);
	ov_clear(sfx->VorbisFile);
	SDL_ClearAudioStream(sfx->Stream);
	SDL_free(sfx);
}

void sgSfxUpdateVolume(sgSfx* sfx, float volume) {
	if (!sfx->Stream || volume > 1.0 || volume < 0) {
		return;
	}
	sfx->Volume = volume;
	SDL_SetAudioStreamGain(sfx->Stream, volume);
}
