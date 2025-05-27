#include <memory>
#include <optional>

#include "AudioEngine.h"
#include "AudioSystem.h"

AudioEngine::AudioEngine()
{
	soloud.init();
}

AudioEngine::~AudioEngine()
{
	soloud.stopAll();
	soloud.deinit();
	currentSounds.clear();
}

void AudioEngine::InitSoundIfNew(const std::string& id, const std::string& path, bool isUnique, float volume)
{
	auto it = currentSounds.find(id);
	if (it == currentSounds.end())
	{
		auto sound = std::make_unique<SoLoud::Wav>();
		sound.get()->setSingleInstance(isUnique);
		sound->load(path.c_str());
		sound->setVolume(volume);
		currentSounds.emplace(std::make_pair(id, std::move(sound)));
	}
}

void AudioEngine::ApplyConfigToSound(const std::string& id, AudioSettings& settings)
{
	if (auto clip = TryGetAudioClip(id))
	{
		SoLoud::Wav& wav = clip->get();
		wav.setLooping(settings.Looping);
		if (settings.handle == -1 && settings.PlayOnAwake)
		{
			settings.handle = soloud.play(wav);
		}

		if (settings.handle != -1)
		{
			if (settings.RequestToPlay)
			{
				auto handle = soloud.play(wav);
				settings.handle = handle;
			}

			if (settings.RequestToStop)
			{
				soloud.stop(settings.handle);
			}

			if (settings.RequestToVolume) 
			{
				soloud.setVolume(settings.handle, settings.desiredVolume);
			}

			if (!soloud.isValidVoiceHandle(settings.handle))
			{
				settings.handle = -1;
			}
		}
		else
		{
			if (settings.RequestToPlay)
			{
				auto handle = soloud.play(wav);
				settings.handle = handle;
			}
		}
	}
}

std::optional<std::reference_wrapper<SoLoud::Wav>> AudioEngine::TryGetAudioClip(const std::string& id)
{
	auto it = currentSounds.find(id);
	if (it != currentSounds.end())
	{
		return *(it->second);
	}
	return std::nullopt;
}