#include "Model.h"
#include "GltfImporter.h"

Model::Model(std::vector<std::shared_ptr<Mesh>> newMeshes)
{
    meshes = newMeshes;
	ComputeExtends();
}

Model::Model(std::vector<std::shared_ptr<Mesh>> newMeshes, AnimationModel newAnimationModel)
{
    meshes = newMeshes;
    animationModel = newAnimationModel;
    ComputeExtends();
}

Model::~Model()
{
    meshes.clear();
}

Vector3 Model::GetExtents() const
{
    return aabb;
}

Vector3 Model::GetCenter() const
{
    return center;
}

std::vector<std::shared_ptr<Mesh>>& Model::GetMeshes()
{
    return meshes;
}

bool Model::HasAnimationModel()
{
    return animationModel ? true : false;
}

AnimationModel& Model::GetAnimationModel()
{
    return animationModel.value();
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
        for (auto& vertex : mesh->vertices)
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
    // Center of the model in local space
    center = (min + max) / 2.0f;
    // We move all vertices relative to that center
    for (auto& mesh : meshes)
    {
        mesh->SetupMesh();
    }
}

Skeleton* AnimationModel::GetSkeleton()
{
    return skeleton.get();
}
