#include "Model.h"
#include "ufbx.h"

Model::Model(const std::string& path)
{
	LoadModel(path);
}

void Model::Draw(Shader& shader)
{
	for (auto i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::LoadModel(std::string path)
{
    ufbx_load_opts opts = { };
    opts.target_axes = ufbx_axes_right_handed_y_up;
    opts.target_unit_meters = 1.0f;
    opts.generate_missing_normals = true;

    ufbx_error error;
    ufbx_scene* scene = ufbx_load_file(path.c_str(), &opts, &error);
    if (!scene) 
    {
        std::cout << "Failed to load scene: %s\n" << error.description.data;
        return;
    }

    for (ufbx_node* node : scene->nodes) 
    {
        std::cout << "NodeName: " << node->name.data << std::endl;
        ProcessNode(node, scene);
    }

    ufbx_free_scene(scene);
}

void Model::ProcessNode(ufbx_node* node, const ufbx_scene* scene)
{
    if (node->mesh)
    {
        ProcessMesh(node->mesh, scene);
        //meshes.push_back(ProcessMesh(node->mesh, scene));
    }

    // Process children
    for (unsigned int i = 0; i < node->children.count; i++)
    {
        ProcessNode(node->children[i], scene);
    }
}

void Model::ProcessMesh(ufbx_mesh* mesh, const ufbx_scene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    // If no materials defined, there will be a single material part for convenience
    for (auto partIndex = 0; partIndex < mesh->material_parts.count; partIndex ++)
    {
        ufbx_mesh_part& part = mesh->material_parts.data[partIndex];
        meshes.push_back(ProcessPart(part, mesh));
    }
}

Mesh Model::ProcessPart(ufbx_mesh_part part, ufbx_mesh* mesh)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    indices.resize(mesh->max_face_triangles * 3);
    for (auto faceIndex : part.face_indices)
    {
        auto face = mesh->faces[faceIndex];
        uint32_t num_tris =
            ufbx_triangulate_face(indices.data(), indices.size(), mesh, face);

        // Iterate over each triangle corner contiguously.
        for (size_t i = 0; i < num_tris * 3; i++) {
            uint32_t index = indices[i];

            Vertex v;
            auto& vertexPosition = mesh->vertex_position[index];
            auto& vertexNormal = mesh->vertex_normal[index];
            v.Position = Vector3(vertexPosition.x, vertexPosition.y, vertexPosition.z);
            v.Normal = Vector3(vertexNormal.x, vertexNormal.y, vertexNormal.z);
            v.TexureCoordinate = Vector3(mesh->vertex_uv[index].x, mesh->vertex_uv[index].y, 0);
            vertices.push_back(v);
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
    return Mesh(vertices, indices, textures);
}
