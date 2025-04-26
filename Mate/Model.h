#pragma once
#include <vector>
#include "ufbx.h"
#include "Shader.h"
#include "Mesh.h"
#include "IModelImporter.h"

class Model : public IRenderable
{
public:
    Model(std::vector<std::shared_ptr<Mesh>> newMeshes);
    ~Model();

    virtual void Draw(Shader& shader) override;

    Vector3 GetExtents()
    {
        return aabb;
    }

private:
    Vector3 aabb;
    std::vector<std::shared_ptr<Mesh>> meshes;

    void ComputeExtends();
};
