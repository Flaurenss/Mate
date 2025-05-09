#pragma once
#include <string>

class ExternalOzzProcessor
{
public:
	static void ProcessGltfModel(std::string modelPath);
private:
	const std::string DefaultProcessorTool = "../dependencies/tools/animations/gltf2ozz.exe";

	static std::string GenerateArguments(std::string modelPath);
};

