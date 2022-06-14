#include "SoundPlayer.h"
#include <AL/alext.h>
#include "stdlib.h"
#include <sndfile.h>
#include "limits.h"


SoundPlayer::SoundPlayer(const char* filename)
{
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
		return;
	}
	if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s\n", filename);
		sf_close(sndfile);
		return;
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
		return;
	}

	/* Decode the whole audio file to a buffer. */
	membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

	num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
	if (num_frames < 1)
	{
		free(membuf);
		sf_close(sndfile);
		fprintf(stderr, "Failed to read samples in %s\n", filename);
		return;
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
		printf("OpenAL Error: %s\n", alGetString(err));
		if (buffer && alIsBuffer(buffer))
			alDeleteBuffers(1, &buffer);
	}

    alGenSources(1, &p_Source);
	p_Buffer = buffer;
	alSourcei(p_Source, AL_BUFFER, p_Buffer);
}

SoundPlayer::~SoundPlayer()
{
	alDeleteSources(1, &p_Source);
}

void SoundPlayer::Play()
{
	alSourcePlay(p_Source);

    if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
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

bool SoundPlayer::isPlaying()
{
	ALint playState;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	return (playState == AL_PLAYING);
}