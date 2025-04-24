#pragma once
#include "ECS.h"
#include "Shader.h"

class RenderSystem : public System
{
public:
	RenderSystem(Shader& shader);

	void Update();
private:
	Shader shader;
};

