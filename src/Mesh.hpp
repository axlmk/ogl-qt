#pragma once

#include <exception>
#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture;
};

struct Texture
{
	unsigned int id = 0;
	TextureType type = TextureType::Diffuse;
	std::filesystem::path location;
};

inline std::string to_string(TextureType t)
{
	if (t == TextureType::Specular)
		return "specular";
	else
		return "diffuse";
}

class Mesh
{
   public:
	// See if they can be private
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void Draw(Shader& shader);

   private:
	unsigned int m_vao, m_ebo, m_vbo;
	void setupMesh();
};
