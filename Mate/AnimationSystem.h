#pragma once
#include "ECS.h"

#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/base/log.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/options/options.h"
#include "Matrix.h"
#include "Skeleton.h"

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

