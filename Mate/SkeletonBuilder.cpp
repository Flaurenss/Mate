#include "SkeletonBuilder.h"
#include "Logger.h"

std::unique_ptr<Skeleton> SkeletonBuilder::BuildFromRaw(const std::vector<RawSkeletonJoint>& rawRoots)
{
    ozz::animation::offline::RawSkeleton rawSkeleton;
    rawSkeleton.roots.resize(rawRoots.size());

    for (size_t i = 0; i < rawRoots.size(); ++i)
    {
        if (!ConvertToOzzJoint(rawRoots[i], rawSkeleton.roots[i]))
        {
            return nullptr;
        }
    }

    if (!rawSkeleton.Validate())
    {
        Logger::Err("Invalid raw skeleton.");
        return nullptr;
    }

    ozz::animation::offline::SkeletonBuilder builder;
    auto ozzSkeleton = builder(rawSkeleton);

    if (!ozzSkeleton)
    {
        Logger::Err("Failed to build skeleton.");
        return nullptr;
    }

    return std::make_unique<Skeleton>(std::move(ozzSkeleton));
}

bool SkeletonBuilder::ConvertToOzzJoint(const RawSkeletonJoint& source, ozz::animation::offline::RawSkeleton::Joint& target)
{
    target.name = source.name.c_str();
    target.transform = ToOzzTransform(source.localTransform);
    target.children.resize(source.children.size());
    for (auto i = 0; i < source.children.size(); i++)
    {
        if (!ConvertToOzzJoint(source.children[i], target.children[i]))
        {
            return false;
        }
    }

    return true;
}

ozz::math::Transform SkeletonBuilder::ToOzzTransform(const Matrix4 matrix)
{
    ozz::math::Transform transform;
    // TODO:
    return transform;
}
