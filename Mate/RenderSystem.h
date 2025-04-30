#pragma once
#include "ECS.h"
#include "Shader.h"
#include "Mesh.h"

class RenderSystem : public System
{
public:
	RenderSystem(Shader& shader);

	void Update();
private:
	Shader shader;

	void BindTexture(Mesh* mesh);
	void DrawMesh(Mesh* mesh);
};

