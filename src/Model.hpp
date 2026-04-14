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

	void translate(const glm::vec3& translation);

	void setPosition(const glm::vec3& position);

	glm::vec3 getPosition() const;

	void scale(float scale);
	void scale(glm::vec3 scale);

	/**
	 * @brief Load the model
	 */
	void loadModel(void);

	glm::mat4 getTransforms(void) const;

   private:
	std::vector<Mesh> m_meshes;
	std::filesystem::path m_objectFile;
	std::filesystem::path m_directory;

	glm::vec3 m_scale;
	glm::vec3 m_position;

	// Change them to references?
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType texType);
};