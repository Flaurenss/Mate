#include "AnimationComponent.h"

void AnimationComponent::ClearCache()
{
	jointTransforms.clear();
}

std::unordered_map<std::string, Matrix4>& AnimationComponent::GetCache()
{
	return jointTransforms;
}
