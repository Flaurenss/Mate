#include "ExternalOzzProcessor.h"
#include <filesystem>
#include <sstream>
#include <iostream>
#include "Logger.h"
#include "AnimationHelper.h"

bool ExternalOzzProcessor::ProcessAnimations(std::string modelPath, std::vector<std::string> animationNames)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();
	std::string fileName = modelFsPath.stem().string();

	std::filesystem::path skeletonPath = GetSkeletonPath(modelFsPath);
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
		std::filesystem::path animPath = GetAnimationPath(modelFsPath, animName);
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

	auto skeletonPath = GetSkeletonPath(modelFsPath);
	auto animationsPath = GetGenericAnimationsPath(modelFsPath);

	std::ostringstream args;
	args << DefaultProcessorTool << " --file=\"" << modelPath << "\" "
		<< "--config=\"{"
		<< "\\\"skeleton\\\":{\\\"filename\\\":\\\"" << skeletonPath << "\\\"},"
		<< "\\\"animations\\\":[{\\\"filename\\\":\\\"" << animationsPath <<"\\\"}]"
		<< "}\"";
	return args.str();
}
