#pragma once
#include "Shader.h"

class IRenderable
{
public:
	virtual void Draw(Shader& shader) = 0;
};

