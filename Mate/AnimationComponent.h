#pragma once
#include <string>
#include <unordered_map>
#include "Matrix.h"

class AnimationComponent
{
public:
    int CurrentAnimationIndex = 0;

    /// <summary>
    /// Get current animation Playback time.
    /// </summary>
    /// <returns>The current animation time.</returns>
    float GetPlaybackTime();

    /// <summary>
    /// Reset the current animation Playback timem to 0.
    /// </summary>
    void ResetPlaybackTime();

    /// <summary>
    /// Increase the current animation Playback time.
    /// </summary>
    /// <param name="delta">The amount to increase.</param>
    void IncreasePlaybackTime(float delta);

    /// <summary>
    /// Get cache of joint names and current transformation.
    /// </summary>
    /// <returns>The actual cache for the current animation.</returns>
    std::unordered_map<std::string, Matrix4>& GetCache();

    /// <summary>
    /// Clear cache of joints trasnforms.
    /// </summary>
    void ClearCache();

private:
    float playbackTime = 0.0f;
    std::unordered_map<std::string, Matrix4> jointTransforms;
};