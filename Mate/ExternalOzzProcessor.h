#pragma once
#include <string>

class ExternalOzzProcessor
{
public:
	static void ProcessGltfModel(std::string modelPath);
private:
	static const std::string DefaultProcessorTool;

	static std::string GenerateArguments(std::string modelPath);
};

