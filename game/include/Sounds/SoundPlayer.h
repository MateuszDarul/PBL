#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__

#include <AL/al.h>

class SoundPlayer
{
public:
	SoundPlayer(const char* filename);
	~SoundPlayer();

	void Play();
	void Stop();
	void Pause();
	void Resume();

	void SetBufferToPlay(const ALuint& buffer_to_play);
	void SetLooping(const bool& loop);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetVolume(float volume);
	void SetPitch(float pitch);

	bool isPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
};

#endif
