#pragma once
#include <string>
#include <filesystem>

const std::string DefaultProcessorTool = ".\\Tools\\animations\\gltf2ozz.exe";
const std::string AnimationPattern = "_anim_";
const std::string SkeletonPattern = "_skeleton";
const std::string OzzExtension = ".ozz";
const std::string DefaultSeparatorPath = "/";

inline std::filesystem::path GetSkeletonPath(std::string modelPath)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	return folder + DefaultSeparatorPath + fileName + SkeletonPattern + OzzExtension;
}

inline std::filesystem::path GetAnimationPath(std::string modelPath, std::string animName)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	return folder + DefaultSeparatorPath + fileName + AnimationPattern + animName + OzzExtension;
}

inline std::filesystem::path GetGenericAnimationsPath(std::string modelPath)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	return folder + DefaultSeparatorPath + fileName + AnimationPattern + "*.ozz";
}

inline std::string GetAnimationConventionName(std::string modelName, std::string animationName)
{
	return modelName + AnimationPattern + animationName + OzzExtension;
}