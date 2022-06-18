#include "AudioManager.h"
#include "SoundPlayer.h"
#include "MusicBuffer.h"

#include <AL/alext.h>
#include <sndfile.h>

#include <stdlib.h>
#include <algorithm>
#include <limits.h>

std::unordered_map<ALuint, std::string> AudioManager::s_LoadedSounds;
std::vector<std::pair<SoundPlayer*, float>> AudioManager::s_SoundsToFadeOut;
std::vector<MusicBuffer*> AudioManager::s_MusicQueue;
unsigned int AudioManager::s_CurrentMusicIndex = -1;
float AudioManager::s_PausedMusicVolumePerUpdate = 1.0f;
float AudioManager::s_TimeToPause = 0.3f;
float AudioManager::s_PauseTimer = -1.0f;
bool AudioManager::s_IsPausing = false;
bool AudioManager::s_IsResuming = false;

ALuint AudioManager::GetSound(const char* filename)
{
	for (auto& [buf, name] : s_LoadedSounds)
	{
		if (name.compare(filename) == 0)
		{
			return buf;
		}
	}

	ALenum err, format;
	ALuint buffer;
	SNDFILE* sndfile;
	SF_INFO sfinfo;
	short* membuf;
	sf_count_t num_frames;
	ALsizei num_bytes;

	/* Open the audio file and check that it's usable. */
	sndfile = sf_open(filename, SFM_READ, &sfinfo);
	if (!sndfile)
	{
		fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
		return 0;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s\n", filename);
		sf_close(sndfile);
		return 0;
	}

	/* Get the sound format, and figure out the OpenAL format */
	format = AL_NONE;
	if (sfinfo.channels == 1)
		format = AL_FORMAT_MONO16;
	else if (sfinfo.channels == 2)
		format = AL_FORMAT_STEREO16;
	else if (sfinfo.channels == 3)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (sfinfo.channels == 4)
	{
		if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			format = AL_FORMAT_BFORMAT3D_16;
	}
	if (!format)
	{
		fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
		sf_close(sndfile);
		return 0;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s\n", filename);
		return 0;
	}
	num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

	/* Buffer the audio data into a new buffer object, then free the data and
	 * close the file.
	 */
	buffer = 0;
	alGenBuffers(1, &buffer);
	alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

	free(membuf);
	sf_close(sndfile);

	/* Check if an error occured, and clean up if so. */
	err = alGetError();
	if (err != AL_NO_ERROR)
	{
		fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
		return 0;
	}

	s_LoadedSounds[buffer] = filename;

	return buffer;
}

void AudioManager::Update(float dt)
{
	for (auto& [sound, amount] : s_SoundsToFadeOut)
	{
		float v = sound->GetCurrentVolume();
		v -= dt * amount;
		if (v < 0.0f)
		{
			sound->Stop();
			amount = -1.0f;
		}
		else
		{
			sound->SetCurrentVolume(v);
		}
	}
	s_SoundsToFadeOut.erase(std::remove_if(s_SoundsToFadeOut.begin(), s_SoundsToFadeOut.end(),
		[&](std::pair<SoundPlayer*, float> item) { return item.second < 0.0f; }),
		s_SoundsToFadeOut.end());


	if (s_MusicQueue.size() > 0)
	{
		if (!s_MusicQueue[s_CurrentMusicIndex]->IsPlaying())
		{
			s_MusicQueue[s_CurrentMusicIndex]->Play();
		}

		s_MusicQueue[s_CurrentMusicIndex]->UpdateBufferStream();

		if (s_IsPausing)
		{
			float v = s_MusicQueue[s_CurrentMusicIndex]->GetCurrentVolume() - dt * s_PausedMusicVolumePerUpdate;
			if (v > 0.0f)
			{
				s_MusicQueue[s_CurrentMusicIndex]->SetCurrentVolume(v);
			}
			else
			{
				s_MusicQueue[s_CurrentMusicIndex]->SetCurrentVolume(0.0f);
				s_MusicQueue[s_CurrentMusicIndex]->Pause();
				s_IsPausing = false;
			}
		}
		else if (s_IsResuming)
		{
			float v = s_MusicQueue[s_CurrentMusicIndex]->GetCurrentVolume() + dt * s_PausedMusicVolumePerUpdate;
			if (v < s_MusicQueue[s_CurrentMusicIndex]->GetVolume())
			{
				s_MusicQueue[s_CurrentMusicIndex]->SetCurrentVolume(v);
			}
			else
			{
				s_MusicQueue[s_CurrentMusicIndex]->SetCurrentVolume(s_MusicQueue[s_CurrentMusicIndex]->GetVolume());
				s_IsResuming = false;
			}
		}
		else if (!s_MusicQueue[s_CurrentMusicIndex]->IsPlaying())
		{
			s_CurrentMusicIndex = (++s_CurrentMusicIndex) % s_MusicQueue.size();
		}
	}
}

void AudioManager::FadeOut(SoundPlayer* sound, float amount)
{
	for (int i = 0; i < s_SoundsToFadeOut.size(); i++)
	{
		if (s_SoundsToFadeOut[i].first == sound)
			return;
	}
	s_SoundsToFadeOut.push_back({ sound, amount });
}

void AudioManager::StopFadeOut(SoundPlayer* sound)
{
	for (int i = 0; i < s_SoundsToFadeOut.size(); i++)
	{
		if (s_SoundsToFadeOut[i].first == sound)
		{
			s_SoundsToFadeOut.erase(s_SoundsToFadeOut.begin() + i, s_SoundsToFadeOut.begin() + i + 1);
			return;
		}
	}
}

void AudioManager::Enqueue(MusicBuffer* music) 
{
	for (int i = 0; i < s_MusicQueue.size(); i++)
	{
		if (s_MusicQueue[i] == music)
			return;
	}

	music->SetLooping(false);
	s_MusicQueue.push_back(music);
	if (s_CurrentMusicIndex == -1)
	{
		s_CurrentMusicIndex = 0;
	}
}

void AudioManager::PauseQueue()
{
	s_PausedMusicVolumePerUpdate = s_MusicQueue[s_CurrentMusicIndex]->GetVolume() / s_TimeToPause;
	s_IsPausing = true;
	s_IsResuming = false;
}

void AudioManager::ResumeQueue()
{
	s_MusicQueue[s_CurrentMusicIndex]->Resume();
	s_IsPausing = false;
	s_IsResuming = true;
}