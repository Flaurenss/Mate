#include "AnimationSystem.h"
#include "MeshComponent.h"
#include "AnimationComponent.h"
#include "EnableComponent.h"
#include "AssetManager.h"

AnimationSystem::AnimationSystem()
{
	RequireComponent<MeshComponent>();
	RequireComponent<AnimationComponent>();
}

void AnimationSystem::Update(float deltaTime)
{
	for (Entity& entity : GetEntities())
	{
		if (entity.HasComponent<EnableComponent>() &&
			!entity.GetComponent<EnableComponent>().Enabled)
		{
			continue;
		}

        auto& meshComp = entity.GetComponent<MeshComponent>();

        // 1. Get the model
        auto model = AssetManager::GetInstance().GetModel(meshComp.GetModelId());
        if (!model || !model->HasAnimationModel())
        {
            continue;
        }

        auto& animModel = model->GetAnimationModel();
        if (animModel.animations.size() == 0)
        {
            continue;
        }

        auto& animComp = entity.GetComponent<AnimationComponent>();
        Skeleton* skeleton = animModel.GetSkeleton();
        auto& clip = animModel.animations[animComp.CurrentAnimationIndex];
        if (!skeleton || !clip)
        {
            continue;
        }

        // Init and get state:
        auto& state = InitAnimationState(entity.GetId(), skeleton);

        // Update playback time
        animComp.IncreasePlaybackTime(deltaTime);
        float duration = clip->GetDuration();
        float timeRatio = fmod(animComp.GetPlaybackTime(), duration) / duration;

        // Animation sampling
        ozz::animation::SamplingJob samplingJob;
        samplingJob.animation = clip->GetAnimation().get();
        samplingJob.context = &state.context;
        samplingJob.ratio = timeRatio;
        samplingJob.output = ozz::make_span(state.locals);
        if (!samplingJob.Run())
        {
            continue;
        }

        // Transform to model space
        ozz::animation::LocalToModelJob ltmJob;
        auto* ozzSkeleton = skeleton->GetOzzSkeleton();
        ltmJob.skeleton = ozzSkeleton;
        ltmJob.input = ozz::make_span(state.locals);
        ltmJob.output = ozz::make_span(state.models);
        if (!ltmJob.Run())
        {
            continue;
        }

        // Store new computed model matrices
        animComp.ClearCache();
        auto& jointMap = animComp.GetCache();
        auto& inverseBind = animModel.inverseBindTransforms;
        
        auto names = ozzSkeleton->joint_names();
        for (int i = 0; i < ozzSkeleton->num_joints(); i++)
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

Matrix4 AnimationSystem::ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix)
{
    float temp[16]{};

    for (int col = 0; col < 4; col++)
    {
        ozz::math::StorePtrU(ozzMatrix.cols[col], temp + col * 4);
    }
    return Matrix4(temp);
}
