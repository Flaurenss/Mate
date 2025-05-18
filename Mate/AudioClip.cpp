#include "AudioClip.h"

AudioClip::AudioClip(std::string audioName, std::string audioPath) : audioName(audioName), audioPath(audioPath)
{ }

std::string AudioClip::GetAudioId() const
{
	return audioName;
}

std::string AudioClip::GetAudioPath() const
{
	return audioPath;
}
