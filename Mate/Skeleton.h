#pragma once
#include <memory>
#include <ozz/animation/runtime/skeleton.h>
#include <ozz/base/maths/simd_math.h>
#include <vector>

class Skeleton
{
public:
    Skeleton(std::shared_ptr<ozz::animation::Skeleton> ozzSkeleton);

    bool IsValid() const;
    const ozz::animation::Skeleton* GetOzzSkeleton() const;

    int GetNumJoints();
    int GetNumSOAJoints();
private:
    std::shared_ptr<ozz::animation::Skeleton> skeleton;
};