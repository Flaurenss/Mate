#include "AnimationBuilder.h"
#include "OzzLoader.h"

void AnimationBuilder::BuildFromFile(const std::filesystem::path& filePath, AnimationModel& animationModel)
{
	auto runtimeUnique = OzzLoader::LoadAnimation(filePath.string());
	animationModel.animations.push_back(std::make_shared<AnimationClip>(filePath.stem().string(), std::move(runtimeUnique)));
}
