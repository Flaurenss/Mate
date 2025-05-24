#define NOMINMAX
#include "AudioSystem.h"
#include "AssetManager.h"
#include "EnableComponent.h"

AudioSystem::AudioSystem()
{
	audioEngine = std::make_unique<AudioEngine>();
	RequireComponent<AudioComponent>();
}

void AudioSystem::Update()
{
	for (Entity& entity : GetEntities())
	{
		if (IsValidForSound(entity))
		{
			auto& audioComponent = entity.GetComponent<AudioComponent>();
			auto audioId = audioComponent.GetAudioId();
			auto audioClip = AssetManager::GetInstance().GetAudioClip(audioId);
			audioEngine->InitSoundIfNew(audioId, audioClip.GetAudioPath(), audioComponent.unique);

			auto audioSettings = AudioSettings
				{
					audioComponent.handleId,
					audioComponent.playOnAwake,
					audioComponent.loop,
					audioComponent.playRequest,
					audioComponent.stopRequest,
					audioComponent.volumeRequest,
					audioComponent.audioVolume
				};
			audioEngine->ApplyConfigToSound(audioId, audioSettings);
			UpdateAudioComponent(audioComponent, audioSettings);
		}
	}
}

bool AudioSystem::IsValidForSound(Entity& entity)
{
	if (entity.HasComponent<EnableComponent>() && !entity.GetComponent<EnableComponent>().Enabled)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void AudioSystem::UpdateAudioComponent(AudioComponent& audioComponent, AudioSettings& settings)
{
	audioComponent.handleId = settings.handle;
	audioComponent.ResetRequests();
}
