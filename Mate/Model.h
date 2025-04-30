#pragma once
#include <vector>
#include "ufbx.h"
#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes);
    ~Model();

    Vector3 GetExtents();
    std::vector<std::shared_ptr<Mesh>> GetMeshes();

private:
    Vector3 aabb;
    std::vector<std::shared_ptr<Mesh>> meshes;

    void ComputeExtends();
};
