#include "Mesh.h"

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices,
	std::vector<Texture> textures) :
		vertices(vertices),
		indices(indices),
		textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// Activate Texture unit
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == DIFFUSE_NAME)
		{
			number = std::to_string(diffuseNr++);
		}
		else if (name == SPECULAR_NAME)
		{
			number = std::to_string(specularNr++);
		}

		shader.SetBool("valid", textures[i].valid);
		if (textures[i].valid)
		{
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
			shader.SetInt(("material." + name + number).c_str(), i);
		}
		else
		{
			shader.SetBool("valid", false);
			shader.SetVec4("defaultColor", textures[i].defaultColor);
		}

		glActiveTexture(GL_TEXTURE0);
	}

	// Draw mesh
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Binds the current VBO to the actual VAO and tells how to be interpret it. 
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	// vertex positions
	// Params are:
	// index of vertex attribute
	// The number of components per vertex attribute: position contains x y z
	// Specify the data type of each element (x y z) -> floats
	// Specify if data needs to be normalized
	// Specify the distance where the same type attribute (for example the position) is repeated in the array
	// Offset  to when starts the first type element. Position are first so is the starter.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	// vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexureCoordinate));
	
	glBindVertexArray(0);
}