#pragma once

#include <AL/al.h>
#include <sndfile.h>

#include <vector>
#include <string>
#include <unordered_map>


class SoundPlayer;
class MusicBuffer;

class AudioManager
{
public:

	static ALuint GetSound(const char* path);

	static void Update(float dt);

	static void Enqueue(MusicBuffer* music);
	static void PauseQueue();
	static void ResumeQueue();
	
	static void ToggleMusic();
	static bool IsMusicPlaying();

	static void SetMasterVolume(float volume);

	static void FadeOut(SoundPlayer* sound, float fadeAmount);
	static void StopFadeOut(SoundPlayer* sound);

private:
	static std::unordered_map<ALuint, std::string> s_LoadedSounds;

	static std::vector<std::pair<SoundPlayer*, float>> s_SoundsToFadeOut;
	static std::vector<MusicBuffer*> s_MusicQueue;
	static unsigned int s_CurrentMusicIndex;

	static float s_PausedMusicVolumePerUpdate;
	static float s_TimeToPause;
	static float s_PauseTimer;
	static bool s_IsPausing;
	static bool s_IsResuming;
	static bool s_IsMusicPaused;
};