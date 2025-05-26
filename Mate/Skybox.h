#pragma once
#include "Shader.h"
#include "Texture.h"

class Skybox
{
public:
	Skybox(std::shared_ptr<Texture> cubeMap);
	~Skybox();

	void Bind();
	unsigned int GetVAO() const;
private:
	std::shared_ptr<Texture> cubeMap;

	unsigned int VAO, VBO;

	void Init();
};

