#pragma once
#include <string>
#include <vector>

class ExternalOzzProcessor
{
public:
	/// <summary>
	/// Process GLTF model with Ozz external tool.
	/// Skeleton and animation files with .ozz extension will be generated only if they do not exist.
	/// </summary>
	/// <param name="modelPath">The 3D model path.</param>
	/// <param name="animationNames">The list of animation names that the model contains.</param>
	/// <returns>True if the import succeeded or was not needed, false if the external tool failed to run.</returns>
	static bool ProcessAnimations(std::string modelPath, std::vector<std::string> animationNames);

private:
	static const std::string DefaultProcessorTool;
	static const std::string AnimationPattern;
	static const std::string SkeletonPattern;
	static const std::string OzzExtension;
	static const std::string PathSpeparator;

	static void ProcessGltfModel(std::string modelPath);
	static std::string GenerateArguments(std::string modelPath);
};

