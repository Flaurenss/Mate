#pragma once

struct AudioSettings
{
	unsigned int handle = -1;
	bool PlayOnAwake = false;
	bool Looping = false;
	bool RequestToPlay = false;
	bool RequestToStop = false;
	bool RequestToVolume = false;
	float desiredVolume = 0.0f;
};