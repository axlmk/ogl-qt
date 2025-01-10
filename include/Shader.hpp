#pragma once

#include "stb_image.h"
#include <filesystem>
#include <fstream>

#include "Utils.hpp"

enum class ShaderType {
	Texture,
	Unicolor,
	VertexColor,
	Custom
};


class Shader {
public:
	
	// TODO : rule of 3
	Shader();
	Shader(ShaderType shaderType);
	// TODO : delete everything
	~Shader();

	void setShaderType(const ShaderType &shaderType);
	ShaderType getShaderType() const;

	void setColor();
	void setTexture(const std::filesystem::path &texturePath);
	void setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	
	void use() const;

private:

	std::string m_err_msg;
	ShaderType m_shaderType;

	unsigned int m_shdPrgId;
	unsigned int m_vtxShd;
	unsigned int m_frgShd;
	unsigned int m_txtBuff;

	void setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	std::string getFileContent(const std::filesystem::path &path);
	
	// TODO : delete everything
	void deleteShaders();
	void deleteTexture();
	void deleteProgram();
};