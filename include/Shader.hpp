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
	Custom,
	Light
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
	void setLight();
	void setTexture(const std::filesystem::path &texturePath, bool isFont=false);
	void setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	
	void setTransformation(glm::mat4 model, glm::mat4 view, glm::mat4 projection);
	int getUniform(std::string) const;

	void compile(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	void use() const;

private:

	struct TextureInfo {
		unsigned char *data;
		int width;
		int height;
		int nrChannels;
	};
	TextureInfo m_textureInfo;
	unsigned int m_txtBuff;
	bool m_font;

	RGBColor m_color;

	ShaderType m_shaderType;
	std::filesystem::path m_vtxShdPath;
	std::filesystem::path m_frgShdPath;
	unsigned int m_shdPrgId;
	std::string m_name;

	std::string getFileContent(const std::filesystem::path &path);
	void setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	
	void deleteShaders(unsigned int vtx, unsigned frg);
	void deleteTexture();
	void deleteProgram();
};