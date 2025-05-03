#pragma once
#include <string>
#include <memory>

#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/runtime/animation.h"

class OzzLoader
{
public:
    static std::unique_ptr<ozz::animation::Skeleton> LoadSkeleton(const std::string& path);
    static std::unique_ptr<ozz::animation::Animation> LoadAnimation(const std::string& path);
};

