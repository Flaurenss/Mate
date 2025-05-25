#pragma once
#include <vector>
#include "Mesh.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include <optional>

struct AnimationModel
{
public:
    std::vector<std::shared_ptr<AnimationClip>> animations;
    std::shared_ptr<Skeleton> skeleton;
    std::unordered_map<std::string, Matrix4> inverseBindTransforms;

    Skeleton* GetSkeleton();
};

class Model
{
public:
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes);
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes, AnimationModel animationModel);
    ~Model();

    Vector3 GetExtents() const;
    Vector3 GetCenter() const;
    std::vector<std::shared_ptr<Mesh>>& GetMeshes();

    bool HasAnimationModel();
    AnimationModel& GetAnimationModel();

private:
    Vector3 aabb;
    Vector3 center;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::optional<AnimationModel> animationModel;

    void ComputeExtends();
};
