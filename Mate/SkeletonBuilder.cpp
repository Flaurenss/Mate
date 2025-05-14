#include "SkeletonBuilder.h"
#include "Logger.h"
#include "OzzLoader.h"

#include <functional>
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"

void SkeletonBuilder::BuildFromFile(const std::string& filePath, AnimationModel& animationModel)
{
    auto ptrSkeleton = OzzLoader::LoadSkeleton(filePath);
    auto skeleton = std::make_shared<Skeleton>(std::move(ptrSkeleton));
    animationModel.skeleton = skeleton;
    animationModel.inverseBindTransforms = GetInverseRestMatrices(skeleton);
}

std::unordered_map<std::string, Matrix4> SkeletonBuilder::GetInverseRestMatrices(std::shared_ptr<Skeleton> skeleton)
{
    std::unordered_map<std::string, Matrix4> inverseMatrices;

    const ozz::animation::Skeleton* ozzSkeleton = skeleton->GetOzzSkeleton();
    const int numJoints = ozzSkeleton->num_joints();
    const int numSoaJoints = ozzSkeleton->num_soa_joints();

    // Convert SoaTransform to AoS transforms
    std::vector<ozz::math::Transform> restTransforms(numJoints);
    for (int i = 0; i < numSoaJoints; i++)
    {
        const ozz::math::SoaTransform& soa = ozzSkeleton->joint_rest_poses()[i];

        ozz::math::SimdFloat4 t[4], r[4], s[4];
        ozz::math::Transpose3x4(&soa.translation.x, t);
        ozz::math::Transpose4x4(&soa.rotation.x, r);
        ozz::math::Transpose3x4(&soa.scale.x, s);

        for (int j = 0; j < 4 && i * 4 + j < numJoints; ++j) {
            auto& dst = restTransforms[i * 4 + j];
            ozz::math::Store3PtrU(t[j], &dst.translation.x);
            ozz::math::StorePtrU(r[j], &dst.rotation.x);
            ozz::math::Store3PtrU(s[j], &dst.scale.x);
        }
    }

    // Store global matrices
    std::vector<Matrix4> globalMatrices(numJoints);
    auto parents = ozzSkeleton->joint_parents();
    for (int i = 0; i < numJoints; i++) {
        Matrix4 localMatrix = ToMatrix(restTransforms[i]);

        if (parents[i] == -1) {
            globalMatrices[i] = localMatrix;
        }
        else {
            globalMatrices[i] = globalMatrices[parents[i]] * localMatrix;
        }
    }

    // Map node names to inverse
    auto names = ozzSkeleton->joint_names();
    for (int i = 0; i < numJoints; i++)
    {
        inverseMatrices[names[i]] = globalMatrices[i].inverse();
    }

    return inverseMatrices;
}

Matrix4 SkeletonBuilder::ToMatrix(const ozz::math::Transform& transform)
{
    const ozz::math::Float3& t = transform.translation;
    const ozz::math::Quaternion& q = transform.rotation;
    const ozz::math::Float3& s = transform.scale;

    const float x = q.x, y = q.y, z = q.z, w = q.w;
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

    // Column-major order (OpenGL-style)
    float m[16] = {
        (1.0f - (yy + zz)) * s.x, (xy + wz) * s.x,        (xz - wy) * s.x,        0.0f,
        (xy - wz) * s.y,        (1.0f - (xx + zz)) * s.y, (yz + wx) * s.y,        0.0f,
        (xz + wy) * s.z,        (yz - wx) * s.z,        (1.0f - (xx + yy)) * s.z, 0.0f,
        t.x,                    t.y,                    t.z,                    1.0f
    };

    return Matrix4(m);
}