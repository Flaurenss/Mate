#pragma once
#include "ECS.h"
#include "AudioEngine.h"
#include "AudioComponent.h"
#include "AudioSettings.h"

class AudioSystem : public System
{
public:
	AudioSystem();
	void Update();
private:
	std::unique_ptr<AudioEngine> audioEngine;

	bool IsValidForSound(Entity& entity);
	void UpdateAudioComponent(AudioComponent& audioComponent, AudioSettings& settings);
};

