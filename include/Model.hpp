#pragma once

#include <filesystem>
#include <vector>

#include <qdebug.h>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"

#include "Shader.hpp"
#include "Mesh.hpp"

class Model
{
	public:
		Model(const std::filesystem::path &path);
		
		void Draw(Shader &shader);

		void translate(const glm::vec3 &translation);

		glm::vec3 getPosition() const;

		void scale(float scale);
		void scale(glm::vec3 scale);

		glm::mat4 getTransforms(void) const;

	private:
	
		std::vector<Mesh> m_meshes;
		std::filesystem::path m_directory;

		glm::vec3 m_scale;
		glm::vec3 m_position;

		void loadModel(const std::filesystem::path &path);
		// Change them to references?
		void processNode(aiNode *node, const aiScene *scene);
		Mesh processMesh(aiMesh *mesh, const aiScene *scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType texType);
};