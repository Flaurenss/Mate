#pragma once
#include <string>
#include <filesystem>
#include "Model.h"

class AnimationBuilder
{
public:
	static void BuildFromFile(const std::filesystem::path&, AnimationModel& animationModel);
};
