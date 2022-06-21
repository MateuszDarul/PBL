#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__

#include <AL/al.h>
#include <glm/vec3.hpp>

class CameraComponent;

class SoundPlayer
{
public:
	SoundPlayer(const char* filename);
	~SoundPlayer();

	void Play();
	void Play3D(const glm::vec3& soundPos, CameraComponent* camera);
	void Stop();
	void Pause();
	void Resume();

	void FadeOut(float time);

	void SetBufferToPlay(const ALuint& buffer_to_play);
	void SetLooping(const bool& loop);
	void SetPosition(const float& x, const float& y, const float& z);
	void SetPitch(float pitch);

	void SetVolume(float volume);
	float GetVolume() const;

	void SetCurrentVolume(float volume);
	float GetCurrentVolume() const;

	bool IsPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;

	float p_Volume = 1.0f;
};

#endif
