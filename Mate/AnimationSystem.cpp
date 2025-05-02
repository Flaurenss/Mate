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

        auto& animComp = entity.GetComponent<AnimationComponent>();
        auto& meshComp = entity.GetComponent<MeshComponent>();

        // 1. Obtener el modelo
        auto model = AssetManager::GetInstance().GetModel(meshComp.GetModelId());
        if (!model || !model->HasAnimationModel())
        {
            continue;
        }

        auto& animModel = model->GetAnimationModel();
        auto skeleton = animModel.GetSkeleton();
        auto& clip = animModel.animations[animComp.CurrentAnimationIndex];
        if (!skeleton || !clip)
        {
            continue;
        }

        // Si no existe aún el estado, inicializar
        auto& state = animationStates[entity.GetId()];
        if (state.locals.empty()) {
            state.context.Resize(skeleton->GetNumJoints());
            state.locals.resize(skeleton->GetNumSOAJoints());
            state.models.resize(skeleton->GetNumJoints());
        }

        // Actualizar tiempo de reproducción
        animComp.PlaybackTime += deltaTime;
        float duration = clip->GetDuration();
        float timeRatio = fmod(animComp.PlaybackTime, duration) / duration;

        // Samplear animación
        ozz::animation::SamplingJob samplingJob;
        samplingJob.animation = clip->GetAnimation().get();
        samplingJob.context = &state.context;
        samplingJob.ratio = timeRatio;
        samplingJob.output = ozz::make_span(state.locals);
        if (!samplingJob.Run())
        {
            continue;
        }

        // Transformar a espacio de modelo
        ozz::animation::LocalToModelJob ltmJob;
        ltmJob.skeleton = skeleton->GetOzzSkeleton();
        ltmJob.input = ozz::make_span(state.locals);
        ltmJob.output = ozz::make_span(state.models);
        if (!ltmJob.Run())
        {
            continue;
        }

        // Store new computed model matrices
        animComp.ClearCache();
        auto& jointMap = animComp.GetCache();
        for (const auto& [jointName, index] : animModel.joinintNameToIndex)
        {
            jointMap[jointName] = ConvertFromOzzMatrix(state.models[index]);
        }
	}
}

Matrix4 AnimationSystem::ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix)
{
    float temp[16];

    for (int col = 0; col < 4; col++)
    {
        ozz::math::StorePtrU(ozzMatrix.cols[col], temp + col * 4);
    }

    return Matrix4(temp);
}
