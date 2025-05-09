#include "ExternalOzzProcessor.h"
#include <filesystem>
#include <sstream>

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
	args << "--file=\"" << modelPath << "\" "
		<< "--config=\"{"
		<< "\\\"skeleton\\\":{\\\"filename\\\":\\\"" << folder << "/skeleton.ozz\\\"},"
		<< "\\\"animations\\\":[{\\\"filename\\\":\\\"" << folder << "/*.ozz\\\"}]"
		<< "}\"";
	return args.str();
}
