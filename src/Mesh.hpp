#pragma once

#include <exception>
#include <glm/glm.hpp>
#include <vector>

#include "Shader.hpp"

/**
 * @brief The data representing a vertex in a mesh
 */
struct Vertex
{
	glm::vec3 position;	 ///< 3D position
	glm::vec3 normal;	 ///< The normal direction
	glm::vec2 texture;	 ///< The 2D location in the texture
};

/**
 * @brief The texture that can be applied onto a mesh
 */
struct Texture
{
	unsigned int id = 0;					  ///< Unique identifier
	TextureType type = TextureType::Diffuse;  ///< Type of texture
	std::filesystem::path location;			  ///< Location on the disk
};

/**
 * @brief Represents the mesh of an object in the 3D scene
 */
class Mesh
{
   public:
	/**
	 * @brief Constructor
	 * @param[in] vertices The list of vertices, points that define the mesh
	 * @param[in] indices The list of indices, linking the vertices
	 * @param[in] textures The list of textures applied onto the mesh
	 */
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

	/**
	 * @brief Draw the mesh onto the screen
	 * @param[in] shader The shader to apply for rendering the mesh
	 */
	void Draw(const Shader& shader) const;

   private:
	/**
	 * @brief Set up all the buffers required to render the mesh
	 */
	void _setupMesh(void);

	std::vector<Vertex> m_vertices;		  ///< The list of vertices, points that define the mesh
	std::vector<unsigned int> m_indices;  ///< The list of indices, linking the vertices
	std::vector<Texture> m_textures;	  ///<

	unsigned int m_vao;	 ///< OpenGL Vertex array object
	unsigned int m_ebo;	 ///< OpenGL Element buffer object
	unsigned int m_vbo;	 ///< OpenGL Vertex buffer object
};
