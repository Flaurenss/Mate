#pragma once
#include <string>
#include "ufbx.h"
#include "Mesh.h"
#include "IModelImporter.h"

class FbxImporterExperimental : public IModelImporter
{
public:
    FbxImporterExperimental() = default;
    ~FbxImporterExperimental() = default;

	virtual std::vector<std::shared_ptr<Mesh>> Load(const std::string& path);

    void ProcessNode(ufbx_node* node, const ufbx_scene* scene);

    void ProcessMesh(ufbx_mesh* mesh, const ufbx_scene* scene, const ufbx_matrix& transform);
    
    std::shared_ptr<Mesh> ProcessPart(ufbx_mesh_part part, ufbx_mesh* mesh, const ufbx_matrix& transform);
    
    std::vector<Texture> LoadMaterialTextures(ufbx_material* material, ufbx_material_pbr_map type,
        std::string typeName);

    unsigned int LoadTexture(const char* path);

private:
    std::vector<std::shared_ptr<Mesh>> meshes;
    std::vector<Texture> loadedTextures;
};

