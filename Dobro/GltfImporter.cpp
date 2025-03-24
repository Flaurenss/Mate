#define CGLTF_IMPLEMENTATION
#define _CRT_SECURE_NO_DEPRECATE
#include "GltfImporter.h"
#include "stb_image.h"
#include <filesystem>

std::vector<Mesh> GltfImporter::Load(const std::string& path)
{
    cgltf_options options = {};
    cgltf_data* data = NULL;
    std::filesystem::path base = std::filesystem::path(path).parent_path();
    basePath = base.string();

    if (cgltf_parse_file(&options, path.c_str(), &data) != cgltf_result_success
        || cgltf_load_buffers(&options, data, path.c_str()) != cgltf_result_success)
    {
        std::cerr << "Failed to load glTF file: " << path << std::endl;
        return meshes;
    }

    cgltf_validate(data);

    for (int nodeIndex = 0; nodeIndex < data->nodes_count; nodeIndex++)
    {
        Matrix4 matrixIdentity;
        ProcessNode(&data->nodes[nodeIndex], matrixIdentity);
    }

    cgltf_free(data);
    return meshes;
}

void GltfImporter::ProcessNode(cgltf_node* node, Matrix4 matrix)
{
    if (node->has_matrix)
    {
        matrix = matrix * Matrix4(
            node->matrix[0], node->matrix[4], node->matrix[8], node->matrix[12],
            node->matrix[1], node->matrix[5], node->matrix[9], node->matrix[13],
            node->matrix[2], node->matrix[6], node->matrix[10], node->matrix[14],
            node->matrix[3], node->matrix[7], node->matrix[11], node->matrix[15]);
    }
    else 
    {
        if (node->has_translation)
        {
            auto translation = node->translation;
            matrix.translate(Vector3(translation[0], translation[1], translation[2]));
        }

        if (node->has_rotation)
        {
            auto rotation = node->rotation;
            // TODO implement quaternions :(
        }

        if (node->has_scale)
        {
            auto scale = node->scale;
            matrix.scale(Vector3(scale[0], scale[1], scale[2]));
        }
    }

    if (node->mesh)
    {
        ProcessMesh(node->mesh, matrix);
    }

    for (size_t chIndex = 0; chIndex < node->children_count; chIndex++)
    {
        ProcessNode(node->children[chIndex], matrix);
    }
}

void GltfImporter::ProcessMesh(cgltf_mesh* mesh, Matrix4 matrix)
{
    for (int i = 0; i < mesh->primitives_count; i++)
    {
        meshes.push_back(ProcessPrimitive(mesh->primitives[i], matrix));
    }
}

Mesh GltfImporter::ProcessPrimitive(cgltf_primitive& primitive, Matrix4 matrix)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices; 
    std::vector<Texture> textures;

    // TODO: do it the proper way, check if attributes any
    size_t vertexCount = primitive.attributes[0].data->count;
    vertices.resize(vertexCount);

    for (size_t i = 0; i < primitive.attributes_count; ++i)
    {
        // TODO understand this assignament
        cgltf_attribute& attr = primitive.attributes[i];
        float* data = (float*)((uint8_t*)attr.data->buffer_view->buffer->data +
            attr.data->buffer_view->offset +
            attr.data->offset);
        
        if (attr.type == cgltf_attribute_type_position)
        {
            for (size_t vertexIndex = 0; vertexIndex < vertexCount; vertexIndex++)
            {
                auto position = matrix * Vector4(data[vertexIndex * 3 + 0], data[vertexIndex * 3 + 1], data[vertexIndex * 3 + 2], 1.0f);
                vertices[vertexIndex].Position = Vector3(position.x, position.y, position.z);
            }
        }
        else if (attr.type == cgltf_attribute_type_normal)
        {
            for (size_t normalIndex = 0; normalIndex < vertexCount; normalIndex++)
            {
                auto normal = matrix * Vector4(data[normalIndex * 3 + 0], data[normalIndex * 3 + 1], data[normalIndex * 3 + 2], 0.0f);
                vertices[normalIndex].Normal = Vector3(normal.x, normal.y, normal.z).normalize();
            }
        }
        else if (attr.type == cgltf_attribute_type_texcoord)
        {
            for (size_t textureIndex = 0; textureIndex < vertexCount; textureIndex++)
            {
                vertices[textureIndex].TexureCoordinate = 
                    Vector2(data[textureIndex * 2 + 0], data[textureIndex * 2 + 1]);
            }
        }
    }

    cgltf_accessor* indexAccessor = primitive.indices;
    for (size_t i = 0; i < indexAccessor->count; ++i)
    {
        auto index = cgltf_accessor_read_index(indexAccessor, i);
        indices.push_back(index);
    }


    // Load textures
    if (primitive.material)
    {
        cgltf_material* mat = primitive.material;

        // TODO: store baseColor as default color
        auto baseColor = mat->pbr_metallic_roughness.base_color_factor;
        auto diffuse = LoadMaterialTextures(mat->pbr_metallic_roughness.base_color_texture.texture, DIFFUSE_NAME);
        auto specular = LoadMaterialTextures(mat->pbr_metallic_roughness.metallic_roughness_texture.texture, SPECULAR_NAME);

        textures.insert(textures.end(), diffuse.begin(), diffuse.end());
        //textures.insert(textures.end(), specular.begin(), specular.end());
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> GltfImporter::LoadMaterialTextures(cgltf_texture* texture, const std::string& typeName)
{
    std::vector<Texture> textures;
    if (!texture || !texture->image || !texture->image->uri)
        return textures;

    std::string texPath = basePath + "/" + texture->image->uri;

    for (const Texture& t : loadedTextures)
    {
        if (t.filePath == texPath)
        {
            textures.push_back(t);
            return textures;
        }
    }

    Texture tex;
    tex.type = typeName;
    tex.filePath = texPath;
    tex.valid = true;
    tex.defaultColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    tex.id = LoadTexture(texPath.c_str());

    loadedTextures.push_back(tex);
    textures.push_back(tex);
    return textures;
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