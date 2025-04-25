#include "DebugDraw.h"
#include <glad/glad.h>
#include <vector>

unsigned int DebugDraw::bboxVAO = 0;
unsigned int DebugDraw::bboxVBO = 0;
unsigned int DebugDraw::axesVAO = 0;
unsigned int DebugDraw::axesVBO = 0;

void DebugDraw::Init()
{
    // Bounding Box (24 vertices for 12 lines)
    float bboxVertices[24 * 3] = {
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

    glGenVertexArrays(1, &bboxVAO);
    glGenBuffers(1, &bboxVBO);
    glBindVertexArray(bboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bboxVertices), bboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // Axes
    float axesVertices[] = {
        0, 0, 0,  1, 0, 0,  // X
        0, 0, 0,  0, 1, 0,  // Y
        0, 0, 0,  0, 0, -1, // Z
        0, 0, 0,  0, -1, 0  // -Y
    };

    glGenVertexArrays(1, &axesVAO);
    glGenBuffers(1, &axesVBO);
    glBindVertexArray(axesVAO);
    glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
}

void DebugDraw::DrawAABB(const Vector3& halfExtents, Matrix4& model, Shader& shader)
{
    shader.Use();
    Matrix4 scale;
    scale.scale(halfExtents);
    Matrix4 finalModel = model * scale;

    shader.SetBool("valid", false);
    shader.SetVec4("defaultColor", Vector4(1.0f, 1.0f, 0.0f, 1.0f));
    shader.SetMat4("model", finalModel);

    glBindVertexArray(bboxVAO);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

void DebugDraw::DrawWorldAxes(Shader& shader)
{
    shader.Use();
    Matrix4 identity;
    identity.identity();
    shader.SetMat4("model", identity);

    glBindVertexArray(axesVAO);

    shader.SetVec4("defaultColor", Vector4(1, 0, 0, 1));
    glDrawArrays(GL_LINES, 0, 2);  // X

    shader.SetVec4("defaultColor", Vector4(0, 1, 0, 1));
    glDrawArrays(GL_LINES, 2, 2);  // Y

    shader.SetVec4("defaultColor", Vector4(0, 0, 1, 1));
    glDrawArrays(GL_LINES, 4, 2);  // Z

    shader.SetVec4("defaultColor", Vector4(0, 1, 0, 1));
    glDrawArrays(GL_LINES, 6, 2);  // -Y

    glBindVertexArray(0);
}

void DebugDraw::Shutdown()
{
    glDeleteVertexArrays(1, &bboxVAO);
    glDeleteBuffers(1, &bboxVBO);
    glDeleteVertexArrays(1, &axesVAO);
    glDeleteBuffers(1, &axesVBO);
}
