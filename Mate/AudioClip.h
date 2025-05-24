#pragma once
#include <string>

class AudioClip
{
public:
	AudioClip(std::string audioName, std::string audioPath);
	~AudioClip() = default;
	
	std::string GetAudioId() const;
	std::string GetAudioPath() const;
private:
	std::string audioName;
	std::string audioPath;
};

