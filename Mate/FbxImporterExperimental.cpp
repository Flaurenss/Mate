#include "FbxImporterExperimental.h"
#include "stb_image.h"
#include "AssetManager.h"

std::vector<std::shared_ptr<Mesh>> FbxImporterExperimental::Load(const std::string& path)
{
    meshes.clear();
    ufbx_load_opts opts = { };
    opts.target_axes = ufbx_axes_right_handed_y_up;
    opts.load_external_files = true;
    opts.target_unit_meters = 1.0f;
    opts.generate_missing_normals = true;

    ufbx_error error;
    ufbx_scene* scene = ufbx_load_file(path.c_str(), &opts, &error);
    if (!scene)
    {
        std::cerr << "Failed to load scene: " << error.description.data;
        return meshes;
    }

    for (ufbx_node* node : scene->nodes)
    {
        std::cout << "NodeName: " << node->name.data << std::endl;
        auto localTransform = node->local_transform;
        ProcessNode(node, scene);
    }

    ufbx_free_scene(scene);

    return meshes;
}

void FbxImporterExperimental::ProcessNode(ufbx_node* node, const ufbx_scene* scene)
{
    ufbx_matrix nodeToWorld = node->node_to_world;
    if (node->mesh)
    {
        ProcessMesh(node->mesh, scene, nodeToWorld);
    }

    // Process children
    for (unsigned int i = 0; i < node->children.count; i++)
    {
        ProcessNode(node->children[i], scene);
    }
}

void FbxImporterExperimental::ProcessMesh(ufbx_mesh* mesh, const ufbx_scene* scene, const ufbx_matrix& transform)
{
    // If no materials defined, there will be a single material part for convenience
    for (auto partIndex = 0; partIndex < mesh->material_parts.count; partIndex++)
    {
        ufbx_mesh_part& part = mesh->material_parts.data[partIndex];
        meshes.push_back(ProcessPart(part, mesh, transform));
    }
}

std::shared_ptr<Mesh> FbxImporterExperimental::ProcessPart(ufbx_mesh_part part, ufbx_mesh* mesh, const ufbx_matrix& transform)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    indices.resize(mesh->max_face_triangles * 3);
    for (auto faceIndex : part.face_indices)
    {
        auto face = mesh->faces[faceIndex];
        uint32_t num_tris =
            ufbx_triangulate_face(indices.data(), indices.size(), mesh, face);

        // Iterate over each triangle corner contiguously.
        for (size_t i = 0; i < num_tris * 3; i++) {
            uint32_t index = indices[i];

            Vertex vertex;
            auto vertexPosition = ufbx_get_vertex_vec3(&mesh->vertex_position, index);
            vertexPosition = ufbx_transform_position(&transform, vertexPosition);
            vertex.Position = Vector3(vertexPosition.x, vertexPosition.y, vertexPosition.z);

            if (mesh->vertex_normal.exists)
            {
                auto vertexNormal = ufbx_get_vertex_vec3(&mesh->vertex_normal, index);
                vertexNormal = ufbx_transform_direction(&transform, vertexNormal);
                vertex.Normal = Vector3(vertexNormal.x, vertexNormal.y, vertexNormal.z);
            }

            if (mesh->vertex_uv.exists)
            {
                auto vertexUV = ufbx_get_vertex_vec2(&mesh->vertex_uv, index);
                vertex.TexureCoordinate = Vector2(vertexUV.x, vertexUV.y);
            }
            else
            {
                vertex.TexureCoordinate = Vector2();
            }
            vertices.push_back(vertex);
        }
    }
    // Should have written all the vertices.
    assert(vertices.size() == part.num_triangles * 3);
    // Generate the index buffer.
    ufbx_vertex_stream streams[1] = {
        { vertices.data(), vertices.size(), sizeof(Vertex) },
    };
    indices.resize(part.num_triangles * 3);
    size_t num_vertices = ufbx_generate_indices(
        streams, 1, indices.data(), indices.size(), nullptr, nullptr);
    // Trim to only unique vertices.
    vertices.resize(num_vertices);

    // Process materials
    if (mesh->materials.count > 0)
    {
        auto material = mesh->materials[part.index];
        auto tex = LoadMaterialTexture(material->pbr.base_color, TextureType::Diffuse);
        if (tex) textures.push_back(tex);
    }
    return std::make_shared<Mesh>(vertices, indices, textures);
}

std::shared_ptr<Texture> FbxImporterExperimental::LoadMaterialTexture(const ufbx_material_map& materialMap, TextureType type)
{
    if (!materialMap.texture)
    {
        return nullptr;
    }

    std::string path = materialMap.texture->filename.data;
    auto tex = AssetManager::GetInstance().LoadTexture(path, path);
    if (tex)
    {
        tex->SetType(type);
    }
    return tex;
}