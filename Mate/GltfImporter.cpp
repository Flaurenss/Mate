#define CGLTF_IMPLEMENTATION
#define _CRT_SECURE_NO_DEPRECATE

#include "GltfImporter.h"
#include "stb_image.h"
#include <filesystem>
#include "AssetManager.h"
#include "Logger.h"
#include "ExternalOzzProcessor.h"

std::shared_ptr<Model> GltfImporter::Load(const std::string& path)
{
    meshes.clear();
    jointNameToIndex.clear();
    baseJointTransforms.clear();
    inverseBindTransforms.clear();

    cgltf_options options = {};
    cgltf_data* data = NULL;
    std::filesystem::path base = std::filesystem::path(path).parent_path();
    basePath = base.string();

    if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success
        || cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success)
    {
        Logger::Err("Failed to load glTF file: " + path);
        return nullptr;
    }

    cgltf_validate(data);

    if (data->scene)
    {
        for (int nodeIndex = 0; nodeIndex < data->scene->nodes_count; nodeIndex++)
        {
            Matrix4 matrixIdentity;
            ProcessNode(&data->nodes[nodeIndex], matrixIdentity);
        }
    }
    else
    {
        Logger::Err("No SCENE defined in : " + path);
    }

    ProcessAnimationData(path, data);
    //ProcessSkins(data);
    //ProcessAnimations(data);
    AnimationModel animationModel;
    animationModel.animations = animationClips;
    animationModel.skeleton = skeleton;
    animationModel.joinintNameToIndex = jointNameToIndex;
    animationModel.baseJointTransforms = baseJointTransforms;
    animationModel.inverseBindTransforms = inverseBindTransforms;

    cgltf_free(data);

    return std::make_shared<Model>(meshes, animationModel);
}

void GltfImporter::ProcessNode(cgltf_node* node, Matrix4 parent)
{
    if (node->has_matrix)
    {
        parent = parent * Matrix4(node->matrix);
    }
    else 
    {
        if (node->has_translation)
        {
            cgltf_float* translation = node->translation;
            parent.translate(Vector3(translation[0], translation[1], translation[2]));
        }

        if (node->has_rotation)
        {
            cgltf_float* quaternion = node->rotation;
            Matrix4 newRotation = Matrix4::ToMatrix(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
            parent = parent * newRotation;
        }

        if (node->has_scale)
        {
            cgltf_float* scale = node->scale;
            parent.scale(Vector3(scale[0], scale[1], scale[2]));
        }
    }

    std::string nodeName = node->name ? node->name : "";

    if (node->mesh)
    {
        for (int i = 0; i < node->mesh->primitives_count; i++)
        {
            auto primitiveMesh = ProcessPrimitive(node->mesh->primitives[i], parent);
            primitiveMesh->attachedJointName = nodeName;
            meshes.push_back(primitiveMesh);
        }
    }

    for (size_t chIndex = 0; chIndex < node->children_count; chIndex++)
    {
        ProcessNode(node->children[chIndex], parent);
    }
}

std::shared_ptr<Mesh> GltfImporter::ProcessPrimitive(cgltf_primitive& primitive, Matrix4 matrix)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; 
    std::vector<std::shared_ptr<Texture>> textures;

    const cgltf_attribute* posAttr = nullptr;
    const cgltf_attribute* normAttr = nullptr;
    const cgltf_attribute* texAttr = nullptr;

    for (size_t i = 0; i < primitive.attributes_count; ++i)
    {
        cgltf_attribute* attr = &primitive.attributes[i];
        if (attr->type == cgltf_attribute_type_position)
        {
            posAttr = attr;
        }
        else if (attr->type == cgltf_attribute_type_normal)
        {
            normAttr = attr;
        }
        else if (attr->type == cgltf_attribute_type_texcoord && attr->index == 0) 
        {
            texAttr = attr;
        }
    }

    // Check if we have positions
    if (!posAttr || !posAttr->data) 
    {
        throw std::runtime_error("Error: Primitive contains no positions.");
    }

    size_t vertexCount = posAttr->data->count;
    vertices.resize(vertexCount);

    for (size_t index = 0; index < vertexCount; ++index)
    {
        Vertex& currentVertex = vertices[index];

        currentVertex.Position = ProcessPosition(posAttr->data, index, matrix);

        // --- Normals ---
        if (normAttr && normAttr->data)
        {
            currentVertex.Normal = ProcessNormal(normAttr->data, index, matrix);
        }

        // --- UVs ---
        if (texAttr && texAttr->data)
        {
            currentVertex.TexureCoordinate = ProcessTextureCoordinate(texAttr->data, index);
        }
    }

    cgltf_accessor* indexAccessor = primitive.indices;
    if (indexAccessor)
    {
        indices.resize(indexAccessor->count);
        for (size_t i = 0; i < indexAccessor->count; ++i)
        {
            indices[i] = cgltf_accessor_read_index(indexAccessor, i);
        }
    }
    else
    {
        //throw std::runtime_error("Error: Primitive contains no indices.");
        std::cout << "INFO: Primitive without index. Generating sequencial indices..." << std::endl;
        indices.resize(vertexCount);
        for (size_t i = 0; i < vertexCount; ++i)
        {
            indices[i] = static_cast<unsigned int>(i);
        }
    }

    // Load textures
    if (primitive.material)
    {
        cgltf_material* mat = primitive.material;
        auto baseColor = mat->pbr_metallic_roughness.base_color_factor;
        auto diffuse = LoadMaterialTextures(mat->pbr_metallic_roughness.base_color_texture.texture, TextureType::Diffuse);
        if (!diffuse)
        {
            // TODO: store default material
        }
        textures.push_back(diffuse);
    }
    else
    {
        // Use default texture
        //Texture texture;
        //texture.id = 0;
        //texture.valid = false;
        //texture.defaultColor = DefaultColor;

        //textures.push_back(texture);
    }

    return std::make_shared<Mesh>(vertices, indices, textures);
}

Vector3 GltfImporter::ProcessPosition(cgltf_accessor* accesor, size_t index, Matrix4 matrix)
{
    float pos[3] = { 0.0f, 0.0f, 0.0f }; // Default
    if (!cgltf_accessor_read_float(accesor, index, pos, 3))
    {
        std::cerr << "Error tor read Vertex Position " << index << std::endl;
    }

    Vector4 worldPos = matrix * Vector4(pos[0], pos[1], pos[2], 1.0f);
    return Vector3(worldPos.x, worldPos.y, worldPos.z);
    //return Vector3(pos[0], pos[1], pos[2]);
}

Vector3 GltfImporter::ProcessNormal(cgltf_accessor* accesor, size_t index, Matrix4 matrix)
{
    float norm[3] = { 0.0f, 0.0f, 1.0f }; // Default
    if (!cgltf_accessor_read_float(accesor, index, norm, 3))
    {
        std::cerr << "Error tor read Vertex Normal" << index << std::endl;
    }
    Vector4 worldNorm = matrix * Vector4(norm[0], norm[1], norm[2], 0.0f);
    return Vector3(worldNorm.x, worldNorm.y, worldNorm.z).normalize();
    //return Vector3(norm[0], norm[1], norm[2]).normalize();
}

Vector2 GltfImporter::ProcessTextureCoordinate(cgltf_accessor* accesor, size_t index)
{
    float uv[2] = { 0.0f, 0.0f }; // Default
    if (!cgltf_accessor_read_float(accesor, index, uv, 2)) {
        std::cerr << "Error to read Vertex Texture Coordinate" << index << std::endl;
    }
    // currentVertex.TexureCoordinate.y = 1.0f - currentVertex.TexureCoordinate.y;
    return Vector2(uv[0], uv[1]);
}

std::shared_ptr<Texture> GltfImporter::LoadMaterialTextures(cgltf_texture* texture, TextureType type)
{
    Texture customTexture;
    if (!texture || !texture->image || !texture->image->uri)
    {
        return nullptr;
    }

    std::string texPath = basePath + "/" + texture->image->uri;

    auto newTexture = AssetManager::GetInstance().LoadTexture(texPath, texPath);

    if (newTexture != nullptr)
    {
        newTexture->SetType(type);
    }

    return newTexture;
}

unsigned int GltfImporter::LoadTexture(const char* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = 
            (nrComponents == 1) ? GL_RED :
            (nrComponents == 3) ? GL_RGB :
            GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Failed to load texture at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void GltfImporter::ProcessAnimationData(const std::string& modelPath, cgltf_data* data)
{
    std::vector<std::string> animationNames;
    for (auto i = 0; i < data->animations_count; i++)
    {
        const cgltf_animation* anim = &data->animations[i];
        animationNames.push_back(anim->name);
    }

    if (animationNames.size() > 0)
    {
        if(ExternalOzzProcessor::ProcessAnimations(modelPath, animationNames))
        {
            auto m = true;
        }
    }
}

void GltfImporter::ProcessAnimations(cgltf_data* data)
{
    animationClips.clear();
    for (auto i = 0; i < data->animations_count; ++i)
    {
        const cgltf_animation* anim = &data->animations[i];
        auto clip = BuildAnimationClip(anim);
        if (clip)
        {
            animationClips.push_back(clip);
        }
    }
}

std::shared_ptr<AnimationClip> GltfImporter::BuildAnimationClip(const cgltf_animation* anim)
{
    std::vector<RawAnimationClip> rawTracks;
    float duration = 0.0f;

    // Iterate each channel which contain specification of animation type (scaling, rotation...)
    for (auto c = 0; c < anim->channels_count; ++c)
    {
        const cgltf_animation_channel& channel = anim->channels[c];
        const cgltf_animation_sampler* sampler = channel.sampler;
        const cgltf_node* node = channel.target_node;
        if (!node || !sampler || !sampler->input || !sampler->output)
        {
            continue;
        }

        // Check if raw clip already inserted
        std::string nodeName = node->name ? node->name : "Unnamed_Node";
        auto it = std::find_if(rawTracks.begin(),
            rawTracks.end(),
            [&](const RawAnimationClip& raw)
            {
                return raw.name == nodeName;
            });

        if (it == rawTracks.end())
        {
            rawTracks.push_back({ nodeName });
            it = rawTracks.end() - 1;
        }

        for (auto k = 0; k < sampler->input->count; ++k)
        {
            float time;
            cgltf_accessor_read_float(sampler->input, k, &time, 1);
            duration = std::max(duration, time);

            if (channel.target_path == cgltf_animation_path_type_translation)
            {
                float translation[3];
                cgltf_accessor_read_float(sampler->output, k, translation, 3);
                it->translations.emplace_back(time, Vector3(translation[0], translation[1], translation[2]));
            }
            else if (channel.target_path == cgltf_animation_path_type_rotation)
            {
                float roation[4]; // As quaternion
                cgltf_accessor_read_float(sampler->output, k, roation, 4);
                it->rotations.emplace_back(time, Vector4(roation[0], roation[1], roation[2], roation[3]));
            }
            else if (channel.target_path == cgltf_animation_path_type_scale)
            {
                float scale[3];
                cgltf_accessor_read_float(sampler->output, k, scale, 3);
                it->scales.emplace_back(time, Vector3(scale[0], scale[1], scale[2]));
            }
        }
    }

    std::string animName = anim->name ? anim->name : "Unnamed_Animation";
    return AnimationClip::BuildFromRawTracks(animName, rawTracks, duration, jointNameToIndex);
}

void GltfImporter::ProcessSkins(cgltf_data* data)
{
    if (data->skins_count > 0)
    {
    }
    else
    {
        Logger::War("Skin reading is not supported. The entire scene graph will be considered as Skeleton.");
    }

    std::vector<RawSkeletonJoint> roots;

    for (int i = 0; i < data->scene->nodes_count; i++)
    {
        RawSkeletonJoint joint = ExtractJointHierarchy(data->scene->nodes[i], Matrix4());
        //ApplySkeletonCorrections(joint);
        roots.push_back(joint);
    }

    auto result = SkeletonBuilder::BuildFromRaw(roots);
    skeleton = result.skeleton;
    jointNameToIndex = result.jointNameToIndex;

    for (const auto& [name, matrix] : baseJointTransforms)
    {
        inverseBindTransforms[name] = matrix.inverse();
    }
}

RawSkeletonJoint GltfImporter::ExtractJointHierarchy(cgltf_node* node, const Matrix4& parentGlobal)
{
    RawSkeletonJoint joint;
    joint.name = node->name ? node->name : "NO_NAME";
    Logger::Log("Node:" + joint.name);

    Matrix4 translation;
    if (node->has_translation)
    {
        translation.translate(Vector3(
            node->translation[0],
            node->translation[1],
            node->translation[2]));
    }

    Matrix4 rotation;
    if (node->has_rotation)
    {
        rotation = Matrix4::ToMatrix(
            node->rotation[0],
            node->rotation[1],
            node->rotation[2],
            node->rotation[3]);
    }

    Matrix4 scale;
    if (node->has_scale)
    {
        scale.scale(Vector3(
            node->scale[0],
            node->scale[1],
            node->scale[2]));
    }

    Matrix4 local;
    if (node->has_matrix)
    {
        local = Matrix4(node->matrix);
    }
    else
    {
        local = translation * rotation * scale;
    }

    Matrix4 globalTransform = parentGlobal * local;

    joint.transformation = local;
    //joint.transformation = globalTransform;
    baseJointTransforms[joint.name] = globalTransform;
    for (int i = 0; i < node->children_count; i++)
    {
        joint.children.push_back(ExtractJointHierarchy(node->children[i], globalTransform));
    }

    return joint;
}