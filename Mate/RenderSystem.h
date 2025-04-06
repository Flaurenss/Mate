#pragma once
#include "ECS.h"
#include "Shader.h"

class RenderSystem : public System
{
public:
	RenderSystem(Shader& shader);

	virtual void Update();
private:
	Shader shader;
};

