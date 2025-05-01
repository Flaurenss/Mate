#pragma once
#include <vector>
#include "Mesh.h"
#include "AnimationClip.h"

class Model
{
public:
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes);
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes, std::vector<std::shared_ptr<AnimationClip>> newAnimations);
    ~Model();

    Vector3 GetExtents();
    std::vector<std::shared_ptr<Mesh>> GetMeshes();

private:
    Vector3 aabb;
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<std::shared_ptr<AnimationClip>> animations;

    void ComputeExtends();
};
