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
    Model(Mesh mesh);
    ~Model();

    virtual void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;
    IModelImporter* modelImporter;
};
