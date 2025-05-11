#include "ExternalOzzProcessor.h"
#include <filesystem>
#include <sstream>

const std::string ExternalOzzProcessor::DefaultProcessorTool = ".\\Tools\\animations\\gltf2ozz.exe";

void ExternalOzzProcessor::ProcessGltfModel(std::string modelPath)
{
	auto command = GenerateArguments(modelPath);
	int result = std::system(command.c_str());
}

std::string ExternalOzzProcessor::GenerateArguments(std::string modelPath)
{
	std::filesystem::path modelFsPath = modelPath;
	std::string folder = modelFsPath.parent_path().string();

	// Escapar comillas dobles dentro del JSON
	std::ostringstream args;
	args << DefaultProcessorTool << " --file=\"" << modelPath << "\" "
		<< "--config=\"{"
		<< "\\\"skeleton\\\":{\\\"filename\\\":\\\"" << folder << "/skeleton.ozz\\\"},"
		<< "\\\"animations\\\":[{\\\"filename\\\":\\\"" << folder << "/*.ozz\\\"}]"
		<< "}\"";
	return args.str();
}
