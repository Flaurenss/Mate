#pragma once

#include "IModelImporter.h"
#include "cgltf.h"
#include <Matrix.h>
#include "Texture.h"
#include "Skeleton.h"
#include "SkeletonBuilder.h"

class GltfImporter : public IModelImporter
{
public:
	GltfImporter() = default;
	~GltfImporter() = default;
	virtual std::shared_ptr<Model> Load(const std::string& path);

private:
	std::string basePath;
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::vector<std::shared_ptr<AnimationClip>> animationClips;
	std::shared_ptr<Skeleton> skeleton;
	std::unordered_map<std::string, int> jointNameToIndex;
	std::unordered_map<std::string, Matrix4> baseJointTransforms;
	std::unordered_map<std::string, Matrix4> inverseBindTransforms;

	void ProcessNode(cgltf_node* node, Matrix4 matrix);
	void ProcessMesh(cgltf_mesh* mesh, Matrix4 matrix);
	std::shared_ptr<Mesh> ProcessPrimitive(cgltf_primitive& primitive, Matrix4 matrix);

	Vector3 ProcessPosition(cgltf_accessor* accesor, size_t index, Matrix4 matrix);
	Vector3 ProcessNormal(cgltf_accessor* accesor, size_t index, Matrix4 matrix);
	Vector2 ProcessTextureCoordinate(cgltf_accessor* accesor, size_t index);

	std::shared_ptr<Texture> LoadMaterialTextures(cgltf_texture* texture, TextureType type);
	unsigned int LoadTexture(const char* path);

	void ProcessAnimationData(cgltf_data* data);
	void ProcessAnimations(cgltf_data* data);
	std::shared_ptr<AnimationClip> BuildAnimationClip(const cgltf_animation* anim);
	void ProcessSkins(cgltf_data* data);
	RawSkeletonJoint ExtractJointHierarchy(cgltf_node* node, const Matrix4& parentGlobal);
};