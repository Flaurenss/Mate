#include "DebugDraw.h"
#include <glad/glad.h>
#include <vector>

unsigned int DebugDraw::VAO = 0;
unsigned int DebugDraw::VBO = 0;

void DebugDraw::Init()
{
    // 12 lines = 24 vertices
    float vertices[24 * 3] = {
        // Bottom square
        -1, -1, -1,   1, -1, -1,
         1, -1, -1,   1, -1,  1,
         1, -1,  1,  -1, -1,  1,
        -1, -1,  1,  -1, -1, -1,

        // Top square
        -1, 1, -1,    1, 1, -1,
         1, 1, -1,    1, 1,  1,
         1, 1,  1,   -1, 1,  1,
        -1, 1,  1,   -1, 1, -1,

        // Vertical lines
        -1, -1, -1,  -1, 1, -1,
         1, -1, -1,   1, 1, -1,
         1, -1,  1,   1, 1,  1,
        -1, -1,  1,  -1, 1,  1
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void DebugDraw::DrawAABB(const Vector3& center, const Vector3& halfExtents, Matrix4& model, Shader& shader)
{
    shader.Use();

    Matrix4 scale;
    scale.scale(halfExtents); // NO dividir entre 2

    Matrix4 finalModel = model * scale; // model ya tiene la posición

    shader.SetBool("valid", false);
    shader.SetVec4("defaultColor", Vector4(1.0f, 1.0f, 0.0f, 1.0f));
    shader.SetMat4("model", finalModel);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

void DebugDraw::Shutdown()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}
