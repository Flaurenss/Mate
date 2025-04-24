#pragma once
#include <vector>
#include "ufbx.h"
#include "Shader.h"
#include "Mesh.h"
#include "IModelImporter.h"

class Model : public IRenderable
{
public:
    Model(const std::string& path);
    Model(std::shared_ptr<Mesh> mesh);
    ~Model();

    virtual void Draw(Shader& shader);

    Vector3 GetExtents()
    {
        return aabb;
    }

private:
    Vector3 aabb;
    std::vector<std::shared_ptr<Mesh>> meshes;
    IModelImporter* modelImporter;

    void ComputeExtends();
};
