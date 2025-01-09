#pragma once

#include <filesystem>
#include <fstream>

#include "Utils.hpp"

class Shader {
public:
	
	Shader();
	Shader(const std::filesystem::path &vtxShdPath, const std::filesystem::path &frgShdPath);
	~Shader();

	void setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath);
	void use() const;

private:

	unsigned int m_shdPrgId;
	unsigned int m_vtxShd;
	unsigned int m_frgShd;
	std::string m_err_msg;

	std::string getFileContent(const std::filesystem::path &path);
	void deleteShaders();
};