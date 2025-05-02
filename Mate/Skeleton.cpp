#include "Skeleton.h"

Skeleton::Skeleton(std::shared_ptr<ozz::animation::Skeleton> ozzSkeleton)
	: skeleton(ozzSkeleton)
{ }

bool Skeleton::IsValid() const
{
	return skeleton != nullptr;
}

const ozz::animation::Skeleton* Skeleton::GetOzzSkeleton() const
{
	return skeleton.get();
}

int Skeleton::GetNumJoints()
{
	return skeleton->num_joints();
}

int Skeleton::GetNumSOAJoints()
{
	return skeleton->num_soa_joints();
}
