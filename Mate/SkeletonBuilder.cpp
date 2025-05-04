#include "SkeletonBuilder.h"
#include "Logger.h"
#include "ozz/base/maths/simd_math.h"
#include <functional>
#include "OzzLoader.h"

SkeletonBuildResult SkeletonBuilder::BuildFromRaw(const std::vector<RawSkeletonJoint>& rawRoots)
{
    auto ptr = OzzLoader::LoadSkeleton("./Assets/Player/skeleton.ozz");
    ozz::animation::offline::RawSkeleton rawSkeleton;
    rawSkeleton.roots.resize(rawRoots.size());
    std::unordered_map<std::string, int> jointNameToIndex;
    int index = 0;
    for (size_t i = 0; i < rawRoots.size(); ++i)
    {
        if (!ConvertToOzzJoint(rawRoots[i], rawSkeleton.roots[i], jointNameToIndex, index))
        {
            return { nullptr, {} };
        }
    }

    if (!rawSkeleton.Validate())
    {
        Logger::Err("Invalid raw skeleton.");
        return { nullptr, {} };
    }

    ozz::animation::offline::SkeletonBuilder builder;
    auto ozzSkeleton = builder(rawSkeleton);

    if (!ozzSkeleton)
    {
        Logger::Err("Failed to build skeleton.");
        return { nullptr, {} };
    }

    return { std::make_shared<Skeleton>(std::move(ptr)), jointNameToIndex };
}

bool SkeletonBuilder::ConvertToOzzJoint(const RawSkeletonJoint& source, ozz::animation::offline::RawSkeleton::Joint& target, std::unordered_map<std::string, int>& jointNameToIndex,
    int& index)
{
    target.name = source.name.c_str();
    jointNameToIndex[std::string(source.name)] = index++;
    target.transform = ToOzzTransform(source);
    target.children.resize(source.children.size());
    for (auto i = 0; i < source.children.size(); i++)
    {
        if (!ConvertToOzzJoint(source.children[i], target.children[i], jointNameToIndex, index))
        {
            return false;
        }
    }

    return true;
}

ozz::math::Transform SkeletonBuilder::ToOzzTransform(const RawSkeletonJoint& source)
{
    ozz::math::Transform transform = ozz::math::Transform::identity();
    
    auto matrixData = source.transformation.get();
    ozz::math::Float4x4 matrix = {
        ozz::math::simd_float4::Load(matrixData[0], matrixData[1], matrixData[2], matrixData[3]),
        ozz::math::simd_float4::Load(matrixData[4], matrixData[5], matrixData[6], matrixData[7]),
        ozz::math::simd_float4::Load(matrixData[8], matrixData[9], matrixData[10], matrixData[11]),
        ozz::math::simd_float4::Load(matrixData[12], matrixData[13], matrixData[14], matrixData[15])
    };

    if (!ozz::math::ToAffine(matrix, &transform))
    {
        Logger::War("Failed to extract transformation from Joint. Default Identity Transformwill be used.");
    }
    return transform;
}
