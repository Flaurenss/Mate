#include "ExternalOzzProcessor.h"
#include <filesystem>
#include <sstream>
#include <iostream>
#include "Logger.h"

const std::string ExternalOzzProcessor::DefaultProcessorTool = ".\\Tools\\animations\\gltf2ozz.exe";
const std::string ExternalOzzProcessor::AnimationPattern = "_anim_";
const std::string ExternalOzzProcessor::SkeletonPattern = "_skeleton";
const std::string ExternalOzzProcessor::OzzExtension = ".ozz";
const std::string ExternalOzzProcessor::PathSpeparator = "/";

bool ExternalOzzProcessor::ProcessAnimations(std::string modelPath, std::vector<std::string> animationNames)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	std::filesystem::path skeletonPath = folder + PathSpeparator + fileName + SkeletonPattern + OzzExtension;
	bool skeletonExists = std::filesystem::exists(skeletonPath);

	if (!skeletonExists)
	{
		Logger::War("Skeleton not found. Import process will start.");
		std::string command = GenerateArguments(modelPath);
		int result = std::system(command.c_str());
		return result == 0;
	}

	bool allAnimationsExist = true;
	for (const std::string& animName : animationNames)
	{
		std::filesystem::path animPath = folder + PathSpeparator + fileName + AnimationPattern + animName + OzzExtension;
		if (!std::filesystem::exists(animPath))
		{
			allAnimationsExist = false;
			Logger::War("Animation " + animPath.string() + "  is missing. Import process will start.");
			break;
		}
	}

	if (allAnimationsExist)
	{
		Logger::War("Skeleton and animations already exists.");
		return true;
	}

	Logger::War("One or more animations are missing. Import process will start.");
	std::string command = GenerateArguments(modelPath);
	int result = std::system(command.c_str());

	return result == 0;
}

void ExternalOzzProcessor::ProcessGltfModel(std::string modelPath)
{
	auto command = GenerateArguments(modelPath);
	int result = std::system(command.c_str());
}

std::string ExternalOzzProcessor::GenerateArguments(std::string modelPath)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	auto fileName = modelFsPath.stem();

	std::ostringstream args;
	args << DefaultProcessorTool << " --file=\"" << modelPath << "\" "
		<< "--config=\"{"
		<< "\\\"skeleton\\\":{\\\"filename\\\":\\\"" << folder << PathSpeparator << fileName << SkeletonPattern << ".ozz\\\"},"
		<< "\\\"animations\\\":[{\\\"filename\\\":\\\"" << folder << __std_fs_set_current_path << fileName << AnimationPattern <<"*.ozz\\\"}]"
		<< "}\"";
	return args.str();
}
