#pragma once
#include "ECS.h"
#include "Matrix.h"
#include "Skeleton.h"
#include "AnimationComponent.h"
#include "AnimationClip.h"
#include "Model.h"

#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/base/maths/soa_transform.h"

struct AnimationState {
	ozz::animation::SamplingJob::Context context;
	std::vector<ozz::math::SoaTransform> locals;
	std::vector<ozz::math::Float4x4> models;
};

struct EntityAnimationData
{
	AnimationModel& animModel;
	AnimationComponent& animComp;
	Skeleton* skeleton;
	std::shared_ptr<AnimationClip> clip;
};

class AnimationSystem : public System
{
public:
	AnimationSystem();

	void Update(float deltaTime);
private:
	std::unordered_map<int, AnimationState> animationStates;

	std::optional<EntityAnimationData> IsValidForAnimation(const Entity& entity);

	AnimationState& InitAnimationState(int entityId, Skeleton* skeleton);
	bool ExecuteSamplingJob(AnimationComponent& animationComponent, std::shared_ptr<AnimationClip> clip, AnimationState& state, float deltaTime);
	bool ExecuteLocalToModelJob(const ozz::animation::Skeleton* skeleton, AnimationState& state);
	void UpdateJointMap(const ozz::animation::Skeleton* skeleton, AnimationModel& animModel, AnimationState& state, std::unordered_map<std::string, Matrix4>& jointMap);

	Matrix4 ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix);
};

