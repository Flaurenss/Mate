#pragma once
#include <Vector.h>
#include <string>
#include <vector>
#include "IRenderable.h"
#include "Shader.h"

struct Vertex {
	Vector3 Position;
	Vector3 Normal;
	Vector3 TexureCoordinate;
};

struct Texture {
	unsigned int id;
	// Diffuse or specular
	std::string type;
};

class Mesh : public IRenderable
{
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	virtual void Draw(Shader& shader);
private:
	const std::string diffuseName = "texture.diffuse";
	const std::string specularName = "texture.specular";

	// Vertex Array Object: stores pointers to elements from VBO
	unsigned int VAO;
	// Vertex Buffer Object: strores vertex attributes
	unsigned int VBO;
	// Element Buffer Object: stores index data
	unsigned int EBO;

	void SetupMesh();
};

