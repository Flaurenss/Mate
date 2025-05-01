#pragma once
#include <string>
#include <vector>
#include "Matrix.h"
#include <ozz/animation/offline/raw_skeleton.h>
#include <ozz/animation/offline/skeleton_builder.h>
#include "Skeleton.h"

struct RawSkeletonJoint
{
    std::string name;
    Matrix4 localTransform;
    std::vector<RawSkeletonJoint> children;
};

class SkeletonBuilder
{
public:
    SkeletonBuilder() = default;
    static std::shared_ptr<Skeleton> BuildFromRaw(const std::vector<RawSkeletonJoint>& rawRoots);

private:
    static bool ConvertToOzzJoint(const RawSkeletonJoint& source, ozz::animation::offline::RawSkeleton::Joint& target);
    static ozz::math::Transform ToOzzTransform(const Matrix4 matrix);
};

