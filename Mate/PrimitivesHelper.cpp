#include "PrimitivesHelper.h"

Model* PrimitivesHelper::CreatePlane()
{
    std::vector<Vertex> vertices = {
        // Position                 // Normal            // TexCoord
        { Vector3(-0.5f, 0.0f,  0.5f), Vector3(0, 1, 0), Vector2(0.0f, 0.0f) }, // Top-left
        { Vector3(0.5f, 0.0f,  0.5f), Vector3(0, 1, 0), Vector2(0.0f, 0.0f) }, // Top-right
        { Vector3(0.5f, 0.0f, -0.5f), Vector3(0, 1, 0), Vector2(0.0f, 0.0f) }, // Bottom-right
        { Vector3(-0.5f, 0.0f, -0.5f), Vector3(0, 1, 0), Vector2(0.0f, 0.0f) }, // Bottom-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,  // First triangle
        2, 3, 0   // Second triangle
    };

    std::vector<Texture> textures; // Empty since textures are not required
    /*textures.push_back();*/
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(vertices, indices, textures);
    return new Model(mesh);
}
