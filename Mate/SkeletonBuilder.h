#pragma once
#include <string>
#include <vector>
#include "Matrix.h"
#include <ozz/animation/offline/raw_skeleton.h>
#include <ozz/animation/offline/skeleton_builder.h>
#include "Skeleton.h"
#include "Model.h"
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

    static void BuildFromFile(const std::string& filePath, AnimationModel& animationModel);

private:
    static std::unordered_map<std::string, Matrix4> GetInverseRestMatrices(std::shared_ptr<Skeleton> skeleton);
    static ozz::math::Float4x4 ToMatrix(const ozz::math::Transform& transform);
    static Matrix4 ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix);
};