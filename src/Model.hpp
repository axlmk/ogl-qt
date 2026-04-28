#pragma once

#include <filesystem>
#include <vector>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "stb_image.h"

/**
 * @brief Represents the model of a 3D object
 */
class Model
{
   public:
	/**
	 * @brief Constructor
	 * @param[in] path Path to the .obj file on the disk
	 */
	Model(const std::filesystem::path& path);

	/**
	 * @brief Draw the 3D model onto the screen
	 * @param[in] shader The shader to apply to the mesh
	 */
	void Draw(const Shader& shader) const;

	/**
	 * @brief Load the model
	 */
	void load(void);

   private:
	/**
	 * @brief Go through the nodes in the file and for each nodes, process the corresponding mesh
	 * @param[in] node The node to traverse
	 * @param[in] scene The scene containing the node
     */
	void processNode(aiNode* node, const aiScene* scene);

	/**
	 * @brief Transform the mesh from an .obj file to something usable by the renderer
	 * @param[in] mesh The mesh to transform
	 * @param[in] scene The scene containing the mesh
	 * @return The newly constructed mesh
	 */
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	/**
	 * @brief Retrive all the textures located alongside the mesh object
	 * @param[in] mat The material of the retrieved texture
	 * @param[in] type The type of texture in the file
	 * @param[in] texType The type of texture known by the renderer
	 * @return The list of all textures
	 */
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType);

	std::vector<Mesh> m_meshes;			 ///< The list of meshes composing the model
	std::filesystem::path m_objectFile;	 ///< The path to the .obj file located on the disk
	std::filesystem::path m_directory;	 ///< The directory containing the .obj file and other files
};