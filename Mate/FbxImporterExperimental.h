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

	virtual std::shared_ptr<Model> Load(const std::string& path);

private:
    std::vector<std::shared_ptr<Mesh>> meshes;

    void ProcessNode(ufbx_node* node, const ufbx_scene* scene);
    void ProcessMesh(ufbx_mesh* mesh, const ufbx_scene* scene, const ufbx_matrix& transform);
    std::shared_ptr<Mesh> ProcessPart(ufbx_mesh_part part, ufbx_mesh* mesh, const ufbx_matrix& transform);
    std::shared_ptr<Texture> LoadMaterialTexture(const ufbx_material_map& materialMap, TextureType type);
};

