#include "AnimationSystem.h"
#include "MeshComponent.h"
#include "AnimationComponent.h"
#include "EnableComponent.h"
#include "AssetManager.h"
#include "ozz/base/maths/transform.h"

inline ozz::math::Float4x4 ToMatrix(const ozz::math::Transform& transform) {
    auto trans = transform.translation;
    auto rot = transform.rotation;
    const ozz::math::Float3& t = ozz::math::Float3(trans.x, trans.y, trans.z);;
    const ozz::math::Quaternion& r = ozz::math::Quaternion(rot);
    const ozz::math::Float3& s = transform.scale;

    // Convert quaternion to rotation matrix
    const float x = r.x, y = r.y, z = r.z, w = r.w;
    const float x2 = x + x, y2 = y + y, z2 = z + z;

    const float xx = x * x2;
    const float xy = x * y2;
    const float xz = x * z2;

    const float yy = y * y2;
    const float yz = y * z2;
    const float zz = z * z2;

    const float wx = w * x2;
    const float wy = w * y2;
    const float wz = w * z2;

    ozz::math::Float4x4 m;
    m.cols[0] = ozz::math::simd_float4::Load(
        (1.0f - (yy + zz)) * s.x,
        (xy + wz) * s.x,
        (xz - wy) * s.x,
        0.0f
    );
    m.cols[1] = ozz::math::simd_float4::Load(
        (xy - wz) * s.y,
        (1.0f - (xx + zz)) * s.y,
        (yz + wx) * s.y,
        0.0f
    );
    m.cols[2] = ozz::math::simd_float4::Load(
        (xz + wy) * s.z,
        (yz - wx) * s.z,
        (1.0f - (xx + yy)) * s.z,
        0.0f
    );
    m.cols[3] = ozz::math::simd_float4::Load(
        t.x, t.y, t.z, 1.0f
    );
    return m;
}

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
        if (animModel.animations.size() == 0)
        {
            continue;
        }
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

        for (auto& soaTransform : state.locals)
        {
            soaTransform.rotation = ozz::math::NormalizeEst(soaTransform.rotation);
        }

        // Transformar a espacio de modelo
        ozz::animation::LocalToModelJob ltmJob;
        auto ozzSkeleton = skeleton->GetOzzSkeleton();
        ltmJob.skeleton = ozzSkeleton;
        ltmJob.input = ozz::make_span(state.locals);
        ltmJob.output = ozz::make_span(state.models);
        if (!ltmJob.Run())
        {
            continue;
        }

        std::vector<ozz::math::Transform> restTransforms;
        {
            restTransforms.resize(ozzSkeleton->num_joints());
            for (int i = 0; i < ozzSkeleton->num_soa_joints(); ++i) {
                const auto& soa = ozzSkeleton->joint_rest_poses()[i];

                ozz::math::SimdFloat4 t[4], r[4], s[4];
                ozz::math::Transpose3x4(&soa.translation.x, t);
                ozz::math::Transpose4x4(&soa.rotation.x, r);
                ozz::math::Transpose3x4(&soa.scale.x, s);

                for (int j = 0; j < 4 && i * 4 + j < ozzSkeleton->num_joints(); ++j) {
                    auto& dst = restTransforms[i * 4 + j];
                    ozz::math::Store3PtrU(t[j], &dst.translation.x);
                    ozz::math::StorePtrU(r[j], &dst.rotation.x);
                    ozz::math::Store3PtrU(s[j], &dst.scale.x);
                }
            }
        }
        const auto parents = ozzSkeleton->joint_parents();
        std::vector<Matrix4> globalRest;
        globalRest.resize(ozzSkeleton->num_joints());


        std::vector<Matrix4> globalRestMatrices(ozzSkeleton->num_joints());

        for (int i = 0; i < ozzSkeleton->num_joints(); ++i)
        {
            // Convertir transform local a matriz
            ozz::math::Float4x4 localMatrix = ToMatrix(restTransforms[i]);

            // Si no tiene padre, es raíz
            if (parents[i] == -1)
            {
                globalRestMatrices[i] = ConvertFromOzzMatrix(localMatrix);
            }
            else
            {
                globalRestMatrices[i] = globalRestMatrices[parents[i]] * ConvertFromOzzMatrix(localMatrix);
            }
        }
        for (const auto& [jointName, index] : animModel.joinintNameToIndex)
        {
            animModel.inverseBindTransforms[jointName] = globalRestMatrices[index].inverse();
        }

        // Store new computed model matrices
        animComp.ClearCache();
        auto& jointMap = animComp.GetCache();
        auto& inverseBind = animModel.inverseBindTransforms;
        auto& base = animModel.baseJointTransforms;
        for (const auto& [jointName, index] : animModel.joinintNameToIndex)
        {
            auto animatedMatrix = ConvertFromOzzMatrix(state.models[index]);
            //jointMap[jointName] = animatedMatrix;
            auto it = inverseBind.find(jointName);
            if (it != inverseBind.end())
            {
                jointMap[jointName] = animatedMatrix * animModel.inverseBindTransforms[jointName];
            }
            else
            {
                jointMap[jointName] = animatedMatrix;
            }
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
    auto result = Matrix4(temp);
    return result;
}
