#include "Model.h"
#include "ufbx.h"
#include "GltfImporter.h"
#include "stb_image.h"

Model::Model(const std::string& path)
{
    modelImporter = new GltfImporter();
    meshes = modelImporter->Load(path);
	ComputeExtends();
}

Model::Model(std::shared_ptr<Mesh> mesh)
{
    modelImporter = nullptr;
    meshes.push_back(mesh);
	ComputeExtends();
}

Model::~Model()
{
    delete modelImporter;
    modelImporter = nullptr;
    meshes.clear();
}

void Model::Draw(Shader& shader)
{
	for (auto i = 0; i < meshes.size(); i++)
	{
		meshes[i]->Draw(shader);
	}
}

void Model::ComputeExtends()
{
    if (meshes.empty())
    {
        aabb = Vector3::Zero;
        return;
    }

    Vector3 min(FLT_MAX, FLT_MAX, FLT_MAX);
    Vector3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

    for (const auto& mesh : meshes)
    {
        for (const Vertex& vertex : mesh->vertices)
        {
            min.x = std::min(min.x, vertex.Position.x);
            min.y = std::min(min.y, vertex.Position.y);
            min.z = std::min(min.z, vertex.Position.z);

            max.x = std::max(max.x, vertex.Position.x);
            max.y = std::max(max.y, vertex.Position.y);
            max.z = std::max(max.z, vertex.Position.z);
        }
    }

    aabb = max - min;
}
