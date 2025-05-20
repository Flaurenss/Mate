#pragma once
#include "ECS.h"
#include "Shader.h"
#include <optional>

class CameraSystem : public System
{
public:
	CameraSystem(Shader& shader, Shader& skyboxShader);
	
	void Update();

	void SetResolution(int w, int h);
private:
	int width;
	int height;

	unsigned int skyboxVAO;
	unsigned int skyboxVBO;
	unsigned int skyboxTextureId;

	Shader shader;
	Shader skyboxShader;
};