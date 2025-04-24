#pragma once
#include "Shader.h"
#include "Matrix.h"
#include "Vector.h"

class DebugDraw
{
public:
    static void Init();
    static void DrawAABB(const Vector3& center, const Vector3& halfExtents, Matrix4& model, Shader& shader);
    static void Shutdown();

private:
    static unsigned int VAO, VBO;
};

