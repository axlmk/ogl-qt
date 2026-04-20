#pragma once

#include <filesystem>
#include <vector>

#include "Mesh.hpp"
#include "Shader.hpp"
#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "stb_image.h"

class Model
{
   public:
	Model(const std::filesystem::path& path);

	void Draw(const Shader& shader) const;

	/**
	 * @brief Load the model
	 */
	void load(void);

   private:
	std::vector<Mesh> m_meshes;
	std::filesystem::path m_objectFile;
	std::filesystem::path m_directory;

	// Change them to references?
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType);
};