#pragma once

#include "IModelImporter.h"
#include "cgltf.h"
#include <Matrix.h>

class GltfImporter : public IModelImporter
{
public:
	GltfImporter() = default;
	~GltfImporter() = default;
	virtual std::vector<std::shared_ptr<Mesh>> Load(const std::string& path);

private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<Texture> loadedTextures;
	std::string basePath;

	void ProcessNode(cgltf_node* node, Matrix4 matrix);
	void ProcessMesh(cgltf_mesh* mesh, Matrix4 matrix);
	std::shared_ptr<Mesh> ProcessPrimitive(cgltf_primitive& primitive, Matrix4 matrix);

	Vector3 ProcessPosition(cgltf_accessor* accesor, size_t index, Matrix4 matrix);
	Vector3 ProcessNormal(cgltf_accessor* accesor, size_t index, Matrix4 matrix);
	Vector2 ProcessTextureCoordinate(cgltf_accessor* accesor, size_t index);

	Texture LoadMaterialTextures(cgltf_texture* texture, const std::string& typeName);
	unsigned int LoadTexture(const char* path);
};