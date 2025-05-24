#pragma once
#include <string>

class AudioComponent
{
public:
	/// <summary>
	/// Creates an AudioComponent to be attached to an Entity.
	/// </summary>
	/// <param name="id">The AudioClip identifier to be related to the one stored in AssetManager</param>
	/// <param name="isPlayOnAwake">If AudioClip should start playing once loaded.</param>
	/// <param name="isLoop">If the AudioClip should be in loop.</param>
	AudioComponent(std::string id, bool isPlayOnAwake = true, bool isLoop = false);

	/// <summary>
	/// Gets the audio identifier. It must be the same as the stored in AssetManager.
	/// </summary>
	/// <returns>The audio identifier.</returns>
	std::string GetAudioId() const;
	/// <summary>
	/// Set if AudioClip should start playing once loaded.
	/// </summary>
	/// <param name="isPlayOnAwake"></param>
	void SetPlayOnAwake(bool isPlayOnAwake);
	/// <summary>
	/// Set if AudioClip should repeat once it ends.
	/// </summary>
	/// <param name="isLoop"></param>
	void SetIsLoop(bool isLoop);

	/// <summary>
	/// Play the assigned Audio Clip. Properties will affect it.
	/// </summary>
	void Play();
	/// <summary>
	/// Stops the current assigned AudioClip;
	/// </summary>
	void Stop();

	/// <summary>
	/// Set the desired volume for the AudioClip.
	/// Valid values goes from 0.0f to 1.0f.
	/// </summary>
	/// <param name="newVolume">The new volume to set.</param>
	void SetVolume(float newVolume);

	/// <summary>
	/// Returns if the AudioClip for this component is currently playing.
	/// </summary>
	/// <returns></returns>
	bool IsPlaying() const;

	void SetIsUnique(bool isUnique);

	friend class AudioSystem;

private:
	std::string audioId;
	unsigned int handleId;
	bool isPlaying;
	bool playRequest;
	bool stopRequest;
	bool volumeRequest;
	bool unique;
	bool playOnAwake;
	bool loop;
	float audioVolume;

	void ResetRequests();
};

