#pragma once
#include "Shader.h"
#include "Matrix.h"
#include "Vector.h"

class DebugDraw
{
public:
    static void Init();
    static void DrawAABB(const Vector3& halfExtents, Matrix4& model, Shader& shader);
    static void DrawWorldAxes(Shader& shader);
    static void Shutdown();

private:
    static unsigned int bboxVAO,
                        bboxVBO,
                        axesVAO,
                        axesVBO;
};

