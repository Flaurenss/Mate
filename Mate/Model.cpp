#include "Model.h"
#include "ufbx.h"
#include "GltfImporter.h"
#include "stb_image.h"

Model::Model(std::vector<std::shared_ptr<Mesh>> newMeshes)
{
    meshes = newMeshes;
	ComputeExtends();
}

Model::~Model()
{
    meshes.clear();
}

Vector3 Model::GetExtents()
{
    return aabb;
}

std::vector<std::shared_ptr<Mesh>> Model::GetMeshes()
{
    return meshes;
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

    // Este es el centro del modelo en el espacio local original
    Vector3 center = (min + max) / 2.0f;

    // Desplazar todos los vértices para centrar el modelo
    for (auto& mesh : meshes)
    {
        for (auto& vertex : mesh->vertices)
        {
            vertex.Position -= center;
        }
        mesh->SetupMesh();
    }
}
