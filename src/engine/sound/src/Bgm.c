#include <SDL3/SDL.h>
#include <SDL3/SDL_log.h>
#include <SupergoonEngine/Bgm.h>
#include <stdlib.h>
#include <vorbis/vorbisfile.h>

#define BGM_NUM_BUFFERS 4
#define BGM_BUFFER_SAMPLES 8192	 // 8kb
#define VORBIS_REQUEST_SIZE 4096

static OggVorbis_File vbfile;
static vorbis_info *vbinfo;

static void setLoopPoints(sgBgm *bgm, double *loopBegin, double *loopEnd) {
	// Retrieve the vorbis comments
	vorbis_comment *vc = ov_comment(&vbfile, -1);
	if (!vc) {
		fprintf(stderr, "Error retrieving vorbis comments\n");
		return;
	}
	const char *start = "LOOPSTART=";
	const char *end = "LOOPEND=";
	for (int i = 0; i < vc->comments; ++i) {
		char *comment = vc->user_comments[i];
		if (strncasecmp(comment, start, strlen(start)) == 0) {
			float startTime = atof(comment + strlen(start));
			*loopBegin = startTime;
		} else if (strncasecmp(comment, end, strlen(end)) == 0) {
			float endTime = atof(comment + strlen(end));
			*loopEnd = endTime;
		}
	}
}

static void getLoopPoints(sgBgm *bgm) {
	double loop_begin = 0, loop_end = 0;

	setLoopPoints(bgm, &loop_begin, &loop_end);

	if ((loop_begin) >= (loop_end)) {
		(loop_end) = 0;
	}
	if (loop_begin && loop_begin > 0) {
		bgm->LoopStart = (int64_t)(loop_begin * vbinfo->rate);
	} else
		bgm->LoopStart = ov_pcm_tell(&vbfile);

	// Loop end needs to be measured against our buffers loading, so they will be multiplied by channels and sizeof.
	// Due to us checking this on every step.
	if (loop_end && loop_end > 0) {
		int64_t sample_offset = (int64_t)(loop_end * vbinfo->rate);
		bgm->LoopEnd = sample_offset * vbinfo->channels * sizeof(short);
	} else
		bgm->LoopEnd = ov_pcm_total(&vbfile, -1) * vbinfo->channels * sizeof(short);
}

static void loadDataToStream(sgBgm *bgm) {
	int request_size = VORBIS_REQUEST_SIZE;
	long total_buffer_bytes_read = 0;
	bool isLooped = false;
	while (total_buffer_bytes_read < BGM_BUFFER_SAMPLES) {
		// Request size should be either the full request size, or until we can fill the buffer.
		request_size = (total_buffer_bytes_read + request_size <= BGM_BUFFER_SAMPLES)
						   ? request_size
						   : BGM_BUFFER_SAMPLES - total_buffer_bytes_read;
		//   Update to not go past the loop end
		request_size = (total_buffer_bytes_read + request_size + bgm->CurrentLoopBytesRead <= bgm->LoopEnd)
						   ? request_size
						   : bgm->LoopEnd - (total_buffer_bytes_read + bgm->CurrentLoopBytesRead);
		if (request_size == 0) {
			// *buff_flags = Buff_Fill_MusicHitLoopPoint;
			ov_pcm_seek_lap(&vbfile, bgm->LoopStart);
			// Convert samples to bytes and set that as our current location in the loop
			bgm->CurrentLoopBytesRead = ov_pcm_tell(&vbfile) * bgm->Channels * sizeof(short);
			isLooped = true;
			break;
			// We are at the end of the loop point.
		}
		int current_pass_bytes_read = ov_read(&vbfile, (char *)bgm->Buffer + total_buffer_bytes_read, request_size, 0, sizeof(short), 1, 0);
		if (current_pass_bytes_read == 0) {
			// We are at the end of the song
			break;
		}
		total_buffer_bytes_read += current_pass_bytes_read;
	}
	int rc = SDL_PutAudioStreamData(bgm->Stream, bgm->Buffer, total_buffer_bytes_read);
	if (rc == -1) {
		printf("Uhoh, failed to put samples in stream: %s\n", SDL_GetError());
		return;
	}
	if (!isLooped) {
		bgm->CurrentLoopBytesRead += total_buffer_bytes_read;
	}
}

void sgBgmInit(sgBgm *bgm) {
	int result = ov_fopen(bgm->Filename, &vbfile);
	if (result != 0) {
		SDL_LogError(0, "Could not open audio in %s: %d\n", bgm->Filename, result);
		return;
	}
	vbinfo = ov_info(&vbfile, -1);
	bgm->SampleRate = vbinfo->rate;
	bgm->Channels = vbinfo->channels;
	bgm->CurrentLoopBytesRead = 0;
	size_t data_read_size = (size_t)(BGM_BUFFER_SAMPLES);
	bgm->Buffer = malloc(data_read_size);
	getLoopPoints(bgm);
	const SDL_AudioSpec srcspec = {SDL_AUDIO_S16LE, bgm->Channels, bgm->SampleRate};
	bgm->Stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &srcspec, NULL, NULL);
	if (bgm->Stream == NULL) {
		printf("Uhoh, stream failed to create: %s\n", SDL_GetError());
	} else {
		// We are ready to use the stream!
	}
	// loadDataToStream(bgm);
	SDL_ResumeAudioStreamDevice(bgm->Stream);
	// We want to load the stream with enough data when we are init.
}

void sgBgmUpdate(sgBgm *bgm) {
	const int minimum_audio = (BGM_BUFFER_SAMPLES * BGM_NUM_BUFFERS) - BGM_BUFFER_SAMPLES;
	while (SDL_GetAudioStreamAvailable(bgm->Stream) < minimum_audio) {
		loadDataToStream(bgm);
	}
}
void sgBgmDestroy(sgBgm *bgm) {
	SDL_free(bgm->Filename);
	SDL_free(bgm);
}