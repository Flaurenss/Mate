#pragma once
#include <vector>
#include "Mesh.h"
#include "AnimationClip.h"
#include "Skeleton.h"
#include <optional>

struct AnimationModel
{
    std::vector<std::shared_ptr<AnimationClip>> animations;
    std::shared_ptr<Skeleton> skeleton;
};

class Model
{
public:
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes);
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes, AnimationModel animationModel);
    ~Model();

    Vector3 GetExtents();
    std::vector<std::shared_ptr<Mesh>>& GetMeshes();

private:
    Vector3 aabb;
    std::vector<std::shared_ptr<Mesh>> meshes;
    // TODO: create Animator structure to store below pointers:
    //std::vector<std::shared_ptr<AnimationClip>> animations;
    //std::shared_ptr<Skeleton> skeleton;
    std::optional<AnimationModel> animationModel;

    void ComputeExtends();
};
