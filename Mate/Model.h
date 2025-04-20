#pragma once
#include <vector>
#include "ufbx.h"
#include "Shader.h"
#include "Mesh.h"
#include "FbxImporterExperimental.h"

class Model : public IRenderable
{
public:
    Model(const std::string& path);
    Model(std::shared_ptr<Mesh> mesh);
    ~Model();

    virtual void Draw(Shader& shader);

private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    IModelImporter* modelImporter;
};
