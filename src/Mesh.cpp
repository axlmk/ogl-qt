#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices	= vertices;
	this->indices	= indices;
	this->textures	= textures;

	setupMesh();
}

void Mesh::setupMesh()
{
	g_opengl.glGenVertexArrays(1, &m_vao);
	g_opengl.glGenBuffers(1, &m_vbo);
	g_opengl.glGenBuffers(1, &m_ebo);
	
	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	g_opengl.glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	g_opengl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
	
	g_opengl.glEnableVertexAttribArray(0);
	g_opengl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	g_opengl.glEnableVertexAttribArray(1);
	g_opengl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	g_opengl.glEnableVertexAttribArray(2);
	g_opengl.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture));

	g_opengl.glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
	unsigned int diffuse = 1;
	unsigned int specular = 1;
	for (auto i = 0; i < textures.size(); i++)
	{
		g_opengl.glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		if (textures[i].type == TextureType::Diffuse)
		{
			number = std::to_string(diffuse++);
		}
		else
		{
			number = std::to_string(specular++);
		}
		int uniform = shader.getUniform("material." + to_string(textures[i].type) + "_" + number);
		g_opengl.glUniform1ui(uniform, i);
		g_opengl.glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	g_opengl.glActiveTexture(GL_TEXTURE0);

	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	g_opengl.glBindVertexArray(0);
}