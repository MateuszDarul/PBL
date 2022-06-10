#ifndef __SOUND_DEVICE_H__
#define __SOUND_DEVICE_H__

#include <AL\alc.h>

class SoundDevice
{
public:
	static SoundDevice* get();

private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* p_ALCDevice;
	ALCcontext* p_ALCContext;

};

#endif
