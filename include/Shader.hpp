#pragma once

#include "stb_image.h"
#include <filesystem>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.hpp"

enum class ShaderType {
	Texture,
	Unicolor,
	VertexColor,
	Custom
};

using RGBColor = glm::vec3;

class Shader {
public:
	
	// TODO : rule of 3
	Shader();
	Shader(ShaderType shaderType);
	// TODO : delete everything
	~Shader();

	void setShaderType(const ShaderType &shaderType);
	ShaderType getShaderType() const;

	void setColor(RGBColor color);
	void setColor(std::string color);
	void setTexture(const std::filesystem::path &texturePath);
	void setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	void setTransformation(glm::mat4 transform);
	void compile();

	void use() const;

private:

	struct TextureInfo {
		unsigned char *data;
		int width;
		int height;
		int nrChannels;
	};
	RGBColor m_color;

	ShaderType m_shaderType;
	std::filesystem::path m_vtxShdPath;
	std::filesystem::path m_frgShdPath;

	unsigned int m_shdPrgId;
	unsigned int m_vtxShd;
	unsigned int m_frgShd;
	unsigned int m_txtBuff;
	TextureInfo m_textureInfo;

	void setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	std::string getFileContent(const std::filesystem::path &path);
	
	// TODO : delete everything
	void deleteShaders();
	void deleteTexture();
	void deleteProgram();
};