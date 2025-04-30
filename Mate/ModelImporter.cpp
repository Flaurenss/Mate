#include "ModelImporter.h"
#include "Logger.h"

ModelImporter::ModelImporter()
{
    gltfImporter = std::make_unique<GltfImporter>();
    //fbxImporter = std::make_unique<FbxImporterExperimental>();
}

IModelImporter* ModelImporter::SelectImporter(const std::string& modelFilePath) const
{
    if (EndsWith(modelFilePath, ".gltf") || EndsWith(modelFilePath, ".glb"))
    {
        return gltfImporter.get();
    }
    else
    {
        auto errMsg = "Unsupported model format: " + modelFilePath;
        Logger::Err(errMsg);
        throw std::runtime_error(errMsg);
    }
}

bool ModelImporter::EndsWith(const std::string& value, const std::string& ending) const
{
    if (ending.size() > value.size())
    {
        return false;
    }

    return std::equal(ending.rbegin(), ending.rend(), value.rbegin());
}

std::vector<std::shared_ptr<Mesh>> ModelImporter::Load(const std::string& modelFilePath) const
{
    auto importer = SelectImporter(modelFilePath);
    return importer->Load(modelFilePath);
}
