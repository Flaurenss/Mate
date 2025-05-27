#include "AudioComponent.h"

AudioComponent::AudioComponent(std::string id, bool isPlayOnAwake, bool isLoop) : 
	audioId(id),
	handleId(-1),
	playOnAwake(isPlayOnAwake),
	loop(isLoop),
	playRequest(false),
	stopRequest(false),
	volumeRequest(false),
	unique(false),
	isPlaying(false),
	audioVolume(1.0f)
{ }

std::string AudioComponent::GetAudioId() const
{
	return audioId;
}

void AudioComponent::SetIsLoop(bool isLoop)
{
	loop = isLoop;
}

void AudioComponent::SetPlayOnAwake(bool isPlayOnAwake)
{
	playOnAwake = isPlayOnAwake;
}

void AudioComponent::SetVolume(float newVolume)
{
	volumeRequest = true;
	audioVolume = newVolume;
}

bool AudioComponent::IsPlaying() const
{
	return isPlaying;
}

void AudioComponent::SetIsUnique(bool isUnique)
{
	unique = isUnique;
}

void AudioComponent::Play()
{
	playRequest = true;
	stopRequest = false;
}

void AudioComponent::Stop()
{
	stopRequest = true;
	playRequest = false;
}

void AudioComponent::ResetRequests()
{
	playRequest = false;
	stopRequest = false;
	volumeRequest = false;
}
