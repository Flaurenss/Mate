#include "AnimationSystem.h"
#include "AssetManager.h"
#include "MeshComponent.h"
#include "EnableComponent.h"

AnimationSystem::AnimationSystem()
{
	RequireComponent<MeshComponent>();
	RequireComponent<AnimationComponent>();
}

void AnimationSystem::Update(float deltaTime)
{
	for (Entity& entity : GetEntities())
	{
        if (auto access = IsValidForAnimation(entity))
        {
            auto& state = InitAnimationState(entity.GetId(), access->skeleton);

            if (ExecuteSamplingJob(access->animComp, access->clip, state, deltaTime))
            {
                auto* ozzSkeleton = access->skeleton->GetOzzSkeleton();
                if (ExecuteLocalToModelJob(ozzSkeleton, state))
                {
                    access->animComp.ClearCache();
                    auto& jointMap = access->animComp.GetCache();

                    UpdateJointMap(ozzSkeleton, access->animModel, state, jointMap);
                }
            }
        }
	}
}

std::optional<EntityAnimationData> AnimationSystem::IsValidForAnimation(const Entity& entity)
{
    if (entity.HasComponent<EnableComponent>() &&
        !entity.GetComponent<EnableComponent>().Enabled)
    {
        return std::nullopt;
    }

    auto& meshComp = entity.GetComponent<MeshComponent>();
    auto model = AssetManager::GetInstance().GetModel(meshComp.GetModelId());
    if (!model || !model->HasAnimationModel())
    {
        return std::nullopt;
    }

    AnimationModel& animationModel = model->GetAnimationModel();
    if (animationModel.animations.empty())
    {
        return std::nullopt;
    }

    AnimationComponent& animationComponent = entity.GetComponent<AnimationComponent>();
    Skeleton* skeleton = animationModel.GetSkeleton();
    if (!skeleton || animationComponent.CurrentAnimationIndex >= animationModel.animations.size())
    {
        return std::nullopt;
    }

    std::shared_ptr<AnimationClip>& clip = animationModel.animations[animationComponent.CurrentAnimationIndex];
    if (!clip)
    {
        return std::nullopt;
    }

    return EntityAnimationData{ animationModel, animationComponent, skeleton, clip };
}

AnimationState& AnimationSystem::InitAnimationState(int entityId, Skeleton* skeleton)
{
    auto& state = animationStates[entityId];
    if (state.locals.empty()) {
        state.context.Resize(skeleton->GetNumJoints());
        state.locals.resize(skeleton->GetNumSOAJoints());
        state.models.resize(skeleton->GetNumJoints());
    }

    return state;
}

bool AnimationSystem::ExecuteSamplingJob(
    AnimationComponent& animationComponent,
    std::shared_ptr<AnimationClip> clip,
    AnimationState& state,
    float deltaTime)
{
    // Update playback time
    animationComponent.IncreasePlaybackTime(deltaTime);
    float duration = clip->GetDuration();
    float timeRatio = fmod(animationComponent.GetPlaybackTime(), duration) / duration;

    // Animation sampling
    ozz::animation::SamplingJob samplingJob;
    samplingJob.animation = clip->GetAnimation().get();
    samplingJob.context = &state.context;
    samplingJob.ratio = timeRatio;
    samplingJob.output = ozz::make_span(state.locals);
    return samplingJob.Run();
}

bool AnimationSystem::ExecuteLocalToModelJob(const ozz::animation::Skeleton* skeleton, AnimationState& state)
{
    // Transform to model space
    ozz::animation::LocalToModelJob ltmJob;
    ltmJob.skeleton = skeleton;
    ltmJob.input = ozz::make_span(state.locals);
    ltmJob.output = ozz::make_span(state.models);
    return ltmJob.Run();
}

void AnimationSystem::UpdateJointMap(const ozz::animation::Skeleton* skeleton, AnimationModel& animModel, AnimationState& state, std::unordered_map<std::string, Matrix4>& jointMap)
{
    auto& inverseBind = animModel.inverseBindTransforms;
    auto names = skeleton->joint_names();
    for (int i = 0; i < skeleton->num_joints(); i++)
    {
        auto animatedMatrix = ConvertFromOzzMatrix(state.models[i]);
        auto name = names[i];
        auto it = inverseBind.find(name);
        if (it != inverseBind.end())
        {
            jointMap[name] = animatedMatrix * inverseBind[name];
        }
        else
        {
            jointMap[name] = animatedMatrix;
        }
    }
}

Matrix4 AnimationSystem::ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix)
{
    float temp[16]{};

    for (int col = 0; col < 4; col++)
    {
        ozz::math::StorePtrU(ozzMatrix.cols[col], temp + col * 4);
    }
    return Matrix4(temp);
}
