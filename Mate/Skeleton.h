#pragma once
#include <memory>
#include <ozz/animation/runtime/skeleton.h>

class Skeleton
{
public:
    Skeleton(std::shared_ptr<ozz::animation::Skeleton> ozzSkeleton);

    bool IsValid() const;
    const ozz::animation::Skeleton* GetOzzSkeleton() const;
private:
    std::shared_ptr<ozz::animation::Skeleton> skeleton;
};

