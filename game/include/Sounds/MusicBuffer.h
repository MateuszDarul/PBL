#ifndef __MUSIC_BUFFER_H__
#define __MUSIC_BUFFER_H__

#include <AL/al.h>
#include <sndfile.h>
class MusicBuffer
{
public:
	void Play();
	void Pause();
	void Resume();
	void Stop();

	void SetLooping(bool looping);
	void SetVolume(float volume);
	float GetVolume() const;

	void SetCurrentVolume(float volume);
	float GetCurrentVolume() const;

	void UpdateBufferStream();

	bool IsPlaying() const;
	bool IsPaused() const;

	ALint getSource();

	MusicBuffer(const char* filename, bool looping = false);
	~MusicBuffer();
private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	int p_NumFinishedBuffers = 0;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	bool p_IsLooping;
	bool p_IsPlaying;
	bool p_IsPaused;

	float p_Volume;

	MusicBuffer() = delete;
};

#endif