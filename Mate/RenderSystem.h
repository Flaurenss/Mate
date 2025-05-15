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

	bool IsValidEntity(Entity& entity);
	void ProcessAnimation(Entity& entity, const std::string& jointName, Matrix4& transform);

	void BindTexture(Mesh* mesh);
	void DrawMesh(Mesh* mesh);
};

