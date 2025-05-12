#pragma once
#include "ECS.h"
#include "Matrix.h"
#include "Skeleton.h"

#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/base/maths/soa_transform.h"

struct AnimationState {
	ozz::animation::SamplingJob::Context context;
	std::vector<ozz::math::SoaTransform> locals;
	std::vector<ozz::math::Float4x4> models;
};

class AnimationSystem : public System
{
public:
	AnimationSystem();

	void Update(float deltaTime);
private:
	std::unordered_map<int, AnimationState> animationStates;

	AnimationState& InitAnimationState(int entityId, Skeleton* skeleton);
	Matrix4 ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix);
};

