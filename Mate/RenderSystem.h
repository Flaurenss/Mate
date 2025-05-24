#pragma once
#include "ECS.h"
#include "Shader.h"
#include "Mesh.h"
#include "RenderContext.h"

class RenderSystem : public System
{
public:
	RenderSystem(Shader& baseShader, Shader& skyboxShader);

	void Update(RenderContext& renderContext);
private:
	Shader baseShader;

	bool IsValidEntity(Entity& entity);
	void ProcessAnimation(Entity& entity, const std::string& jointName, Matrix4& transform);

	void BindTexture(Mesh* mesh);
	void DrawMesh(Mesh* mesh);
	void DrawSkybox();
};

