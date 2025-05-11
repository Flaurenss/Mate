#pragma once
#include <string>
#include <filesystem>

const std::string DefaultProcessorTool = ".\\Tools\\animations\\gltf2ozz.exe";
const std::string AnimationPattern = "_anim_";
const std::string SkeletonPattern = "_skeleton";
const std::string OzzExtension = ".ozz";
const std::string DefaultSeparatorPath = "/";

inline std::filesystem::path GetSkeletonPath(std::filesystem::path modelFsPath)
{
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	return folder + DefaultSeparatorPath + fileName + SkeletonPattern + OzzExtension;
}

inline std::filesystem::path GetAnimationPath(std::filesystem::path modelFsPath, std::string animName)
{
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	return folder + DefaultSeparatorPath + fileName + AnimationPattern + animName + OzzExtension;
}

inline std::filesystem::path GetGenericAnimationsPath(std::filesystem::path modelFsPath)
{
	std::string folder = modelFsPath.parent_path().string();
	auto fileName = modelFsPath.stem();
	return folder + DefaultSeparatorPath + fileName.string() + AnimationPattern + "*.ozz";
}

inline std::string GetAnimationConventionName(std::string modelName, std::string animationName)
{
	return modelName + AnimationPattern + animationName + OzzExtension;
}