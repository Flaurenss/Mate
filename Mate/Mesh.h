#pragma once
#include <Vector.h>
#include <string>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"
#include "Texture.h"

struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector2 TexureCoordinate;
};

class Mesh
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<std::shared_ptr<Texture>> textures;

	Mesh(std::vector<Vertex> vertices,
		std::vector<unsigned int> indices,
		std::vector<std::shared_ptr<Texture>> textures);
	~Mesh();

	void SetupMesh();

	unsigned int GetVAO();
	int GetIndexCount();

private:

	// Vertex Array Object: stores pointers to elements from VBO
	unsigned int VAO;
	// Vertex Buffer Object: strores vertex attributes
	unsigned int VBO;
	// Element Buffer Object: stores index data
	unsigned int EBO;

};

