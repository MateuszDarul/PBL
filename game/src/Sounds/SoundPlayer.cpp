#include "SoundPlayer.h"
#include "AudioManager.h"

#include <AL/alext.h>
#include "stdlib.h"
#include <sndfile.h>
#include "limits.h"


SoundPlayer::SoundPlayer(const char* filename)
{
	p_Buffer = AudioManager::GetSound(filename);
    
	alGenSources(1, &p_Source);
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundPlayer::~SoundPlayer()
{
	alDeleteSources(1, &p_Source);
}

void SoundPlayer::Play()
{
	SetCurrentVolume(p_Volume);
	alSourcePlay(p_Source);

	auto err = alGetError();
    if (err != AL_NO_ERROR)
	{
		//throw("error with al");
	}

	AudioManager::StopFadeOut(this);
}

void SoundPlayer::Stop()
{
	alSourceStop(p_Source);
}

void SoundPlayer::Pause()
{
	alSourcePause(p_Source);
}

void SoundPlayer::Resume()
{
	alSourcePlay(p_Source);
}

void SoundPlayer::SetBufferToPlay(const ALuint& buffer_to_play)
{
	if (buffer_to_play != p_Buffer)
	{
		p_Buffer = buffer_to_play;
		alSourcei(p_Source, AL_BUFFER, (ALint)p_Buffer);
	}
}

void SoundPlayer::SetLooping(const bool& loop)
{
	alSourcei(p_Source, AL_LOOPING, (ALint)loop);
}

void SoundPlayer::SetPosition(const float& x, const float& y, const float& z)
{
	alSource3f(p_Source, AL_POSITION, x, y, z);
}

bool SoundPlayer::IsPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}

void SoundPlayer::SetVolume(float volume)
{
	p_Volume = volume;
	alSourcef(p_Source, AL_GAIN, volume);
}

void SoundPlayer::SetPitch(float pitch)
{
	alSourcef(p_Source, AL_PITCH, pitch);
}

float SoundPlayer::GetVolume() const
{
	return p_Volume;
}

void SoundPlayer::SetCurrentVolume(float volume)
{
	alSourcef(p_Source, AL_GAIN, volume);
}

float SoundPlayer::GetCurrentVolume() const
{
	float result = 0.0f;
	alGetSourcef(p_Source, AL_GAIN, &result);
	return result;
}

void SoundPlayer::FadeOut(float time)
{
	float amount = GetCurrentVolume() / time;
	AudioManager::FadeOut(this, amount);
}