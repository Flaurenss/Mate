#pragma once
#include "ECS.h"
#include "Shader.h"
#include <optional>
#include "RenderContext.h"

class CameraSystem : public System
{
public:
	CameraSystem(Shader& shader, Shader& skyboxShader);
	
	void Update(RenderContext& renderContext);

private:
	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	unsigned int skyboxTextureId;

	Shader shader;
	Shader skyboxShader;
};