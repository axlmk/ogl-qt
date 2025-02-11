#pragma once

#include <filesystem>
#include <fstream>

#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Utils.hpp"

enum class ShaderType {
	Texture,
	Unicolor,
	Custom
};

using RGBColor = glm::vec3;

class Shader {
public:
	
	Shader();
	Shader(ShaderType shaderType);
	~Shader();

	void setShaderType(const ShaderType &shaderType);
	ShaderType getShaderType() const;

	void setColor(RGBColor color);
	void setColor(std::string color);

	void setTexture(const std::filesystem::path &texturePath, bool isFont=false);
	void setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	
	void setTransformation(glm::mat4 transform);
	int getUniform(std::string) const;

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
	bool m_font;
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