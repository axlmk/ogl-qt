#include "Model.hpp"

Model::Model(const std::filesystem::path& path) : m_directory{ path.parent_path() }, m_scale{ glm::vec3(1.0f) }
{
	if(!std::filesystem::exists(path))
	{
		throw std::invalid_argument("File [" + path.string() + "] doesn't exist.");
	}
	this->loadModel(path);
}

void Model::Draw(Shader &shader)
{
	for(auto &mesh : m_meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::loadModel(const std::filesystem::path &path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path.string(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		qCritical() << "Assimp:" << importer.GetErrorString();
		throw std::runtime_error("File [" + path.string() + "] could not be loaded.");
	}

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex>			vertices;
	vertices.reserve(mesh->mNumVertices);
	std::vector<Texture>		textures;
	std::vector<unsigned int>	indices;

	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex{};
		vertex.position.x	= mesh->mVertices[i].x;
		vertex.position.y	= mesh->mVertices[i].y;
		vertex.position.z	= mesh->mVertices[i].z;
		
		vertex.normal.x		= mesh->mNormals[i].x;
		vertex.normal.y		= mesh->mNormals[i].y;
		vertex.normal.z		= mesh->mNormals[i].z;
		
		if(mesh->mTextureCoords[0])
		{
			vertex.texture.x = mesh->mTextureCoords[0][i].x;
			vertex.texture.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.texture = {0, 0};
		}
		vertices.push_back(vertex);
	}

	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::Diffuse);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::Specular);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType texType)
{
	unsigned int textureCount = mat->GetTextureCount(type);
	std::vector<Texture> textures;
	textures.reserve(textureCount);
	for(unsigned int i = 0; i < textureCount; i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = TextureFromFile(m_directory / str.C_Str(), texType);
		texture.type = texType;
		texture.location = std::filesystem::path(str.C_Str());
		textures.push_back(texture);
	}
	return textures;
}


void Model::scale(float scale) {
	m_scale = glm::vec3(scale);
}

void Model::scale(glm::vec3 scale) {
	m_scale = scale;
}

glm::mat4 Model::getTransforms(void) const
{
	return glm::scale(glm::mat4(1.0f), m_scale);
}