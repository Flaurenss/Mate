#include "AnimationComponent.h"

float AnimationComponent::GetPlaybackTime()
{
	return playbackTime;
}

void AnimationComponent::ResetPlaybackTime()
{
	playbackTime = 0.0f;
}

void AnimationComponent::IncreasePlaybackTime(float delta)
{
	playbackTime += delta;
}

void AnimationComponent::ClearCache()
{
	jointTransforms.clear();
}

std::unordered_map<std::string, Matrix4>& AnimationComponent::GetCache()
{
	return jointTransforms;
}
