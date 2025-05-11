#include "SkeletonBuilder.h"
#include "Logger.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include <functional>
#include "OzzLoader.h"

SkeletonBuildResult SkeletonBuilder::BuildFromRaw(const std::vector<RawSkeletonJoint>& rawRoots)
{
    auto ptr = OzzLoader::LoadSkeleton("./Assets/Player/skeleton.ozz");
    ozz::animation::offline::RawSkeleton rawSkeleton;
    rawSkeleton.roots.resize(rawRoots.size());
    std::unordered_map<std::string, int> jointNameToIndex;
    int index = 0;
    for (size_t i = 0; i < rawRoots.size(); ++i)
    {
        if (!ConvertToOzzJoint(rawRoots[i], rawSkeleton.roots[i], jointNameToIndex, index))
        {
            return { nullptr, {} };
        }
    }

    if (!rawSkeleton.Validate())
    {
        Logger::Err("Invalid raw skeleton.");
        return { nullptr, {} };
    }

    ozz::animation::offline::SkeletonBuilder builder;
    auto ozzSkeleton = builder(rawSkeleton);

    if (!ozzSkeleton)
    {
        Logger::Err("Failed to build skeleton.");
        return { nullptr, {} };
    }

    return { std::make_shared<Skeleton>(std::move(ptr)), jointNameToIndex };
}

void SkeletonBuilder::BuildFromFile(const std::string& filePath, AnimationModel& animationModel)
{
    auto ptrSkeleton = OzzLoader::LoadSkeleton(filePath);
    auto skeleton = std::make_shared<Skeleton>(std::move(ptrSkeleton));
    animationModel.skeleton = skeleton;
    animationModel.inverseBindTransforms = GetInverseRestMatrices(skeleton);
}

std::unordered_map<std::string, Matrix4> SkeletonBuilder::GetInverseRestMatrices(std::shared_ptr<Skeleton> skeleton)
{
    std::unordered_map<std::string, Matrix4> map;
    const ozz::animation::Skeleton* ozzSkeleton = skeleton->GetOzzSkeleton();
    const auto parents = ozzSkeleton->joint_parents();

    std::vector<ozz::math::Transform> restTransforms;
    {
        restTransforms.resize(ozzSkeleton->num_joints());
        for (int i = 0; i < ozzSkeleton->num_soa_joints(); i++) {
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
    std::vector<Matrix4> globalRestMatrices(ozzSkeleton->num_joints());
    for (int i = 0; i < ozzSkeleton->num_joints(); i++)
    {
        ozz::math::Float4x4 localMatrix = ToMatrix(restTransforms[i]);

        // If no parent then it is root:
        if (parents[i] == -1)
        {
            globalRestMatrices[i] = ConvertFromOzzMatrix(localMatrix);
        }
        else
        {
            globalRestMatrices[i] = globalRestMatrices[parents[i]] * ConvertFromOzzMatrix(localMatrix);
        }
    }
    
    std::unordered_map<std::string, int> joinintNameToIndex;
    auto names = ozzSkeleton->joint_names();

    for (int i = 0; i < ozzSkeleton->num_joints(); i++) {
        joinintNameToIndex[names[i]] = i;
    }

    for (const auto& [jointName, index] : joinintNameToIndex)
    {
        map[jointName] = globalRestMatrices[index].inverse();
    }

    return map;
}

bool SkeletonBuilder::ConvertToOzzJoint(const RawSkeletonJoint& source, ozz::animation::offline::RawSkeleton::Joint& target, std::unordered_map<std::string, int>& jointNameToIndex,
    int& index)
{
    target.name = source.name.c_str();
    jointNameToIndex[std::string(source.name)] = index++;
    target.transform = ToOzzTransform(source);
    target.children.resize(source.children.size());
    for (auto i = 0; i < source.children.size(); i++)
    {
        if (!ConvertToOzzJoint(source.children[i], target.children[i], jointNameToIndex, index))
        {
            return false;
        }
    }

    return true;
}

ozz::math::Transform SkeletonBuilder::ToOzzTransform(const RawSkeletonJoint& source)
{
    ozz::math::Transform transform = ozz::math::Transform::identity();
    
    auto matrixData = source.transformation.get();
    ozz::math::Float4x4 matrix = {
        ozz::math::simd_float4::Load(matrixData[0], matrixData[1], matrixData[2], matrixData[3]),
        ozz::math::simd_float4::Load(matrixData[4], matrixData[5], matrixData[6], matrixData[7]),
        ozz::math::simd_float4::Load(matrixData[8], matrixData[9], matrixData[10], matrixData[11]),
        ozz::math::simd_float4::Load(matrixData[12], matrixData[13], matrixData[14], matrixData[15])
    };

    if (!ozz::math::ToAffine(matrix, &transform))
    {
        Logger::War("Failed to extract transformation from Joint. Default Identity Transformwill be used.");
    }
    return transform;
}

ozz::math::Float4x4 SkeletonBuilder::ToMatrix(const ozz::math::Transform& transform) {
    auto trans = transform.translation;
    auto rot = transform.rotation;
    const ozz::math::Float3& t = ozz::math::Float3(trans.x, trans.y, trans.z);
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

Matrix4 SkeletonBuilder::ConvertFromOzzMatrix(const ozz::math::Float4x4& ozzMatrix)
{
    float temp[16];

    for (int col = 0; col < 4; col++)
    {
        ozz::math::StorePtrU(ozzMatrix.cols[col], temp + col * 4);
    }
    auto result = Matrix4(temp);
    return result;
}