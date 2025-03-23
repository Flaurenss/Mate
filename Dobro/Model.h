#pragma once
#include "Shader.h"
#include "Mesh.h"
#include <vector>
#include "ufbx.h"

class Model : public IRenderable
{
public:
    Model(const std::string& path);
    virtual void Draw(Shader& shader);
private:
    std::string modelPath;
    std::vector<Mesh> meshes;

    void LoadModel(std::string path);
    void ProcessNode(ufbx_node* node, const ufbx_scene* scene);
    void ProcessMesh(ufbx_mesh* mesh, const ufbx_scene* scene);
    Mesh ProcessPart(ufbx_mesh_part part, ufbx_mesh* mesh);
    std::vector<Texture> LoadMaterialTextures(ufbx_material* mat, ufbx_material_pbr_map type,
        std::string typeName);
};
