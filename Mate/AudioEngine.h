#pragma once

#include "AudioSettings.h"

#include <unordered_map>
#include <string>
#include <optional>

#include "soloud/soloud.h"
#include "soloud/soloud_wav.h"

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();

	void InitSoundIfNew(const std::string& id, const std::string& path, bool isUnique);
	void ApplyConfigToSound(const std::string& id, AudioSettings& settings);
private:
	SoLoud::Soloud soloud; // SoLoud engine core

	std::optional<std::reference_wrapper<SoLoud::Wav>> TryGetAudioClip(const std::string& id);
	std::unordered_map<std::string, std::unique_ptr<SoLoud::Wav>> currentSounds;
};

