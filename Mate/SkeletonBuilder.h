#pragma once
#include <string>
#include <vector>
#include "Matrix.h"
#include <ozz/animation/offline/raw_skeleton.h>
#include <ozz/animation/offline/skeleton_builder.h>
#include "Skeleton.h"
#include <unordered_map>


struct SkeletonBuildResult
{
    std::shared_ptr<Skeleton> skeleton;
    std::unordered_map<std::string, int> jointNameToIndex;
};

struct RawSkeletonJoint
{
    std::string name;
    Matrix4 transformation;
    std::vector<RawSkeletonJoint> children;
};

class SkeletonBuilder
{
public:
    SkeletonBuilder() = default;
    static SkeletonBuildResult BuildFromRaw(const std::vector<RawSkeletonJoint>& rawRoots);

private:
    static bool ConvertToOzzJoint(
        const RawSkeletonJoint& source,
        ozz::animation::offline::RawSkeleton::Joint& target,
        std::unordered_map<std::string, int>& jointNameToIndex,
        int& index);
    static ozz::math::Transform ToOzzTransform(const RawSkeletonJoint& source);
};