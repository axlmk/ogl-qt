#include <Shader.hpp>

std::map<std::filesystem::path, LoadedTextures> g_loadedTextures;

Shader::Shader() : Shader(ShaderType::Custom) {}

Shader::Shader(ShaderType shaderType, bool isFont) : m_shaderType{shaderType}, m_shdPrgId{}, m_font{isFont}, m_color{}
{}

Shader::~Shader()
{
	if (m_shdPrgId)
	{
		g_opengl.glDeleteProgram(m_shdPrgId);
		m_shdPrgId = 0;
	}
}

void Shader::setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath)
{
	if (m_shaderType != ShaderType::Custom)
	{
		std::string err_msg = "Shader type must be 'custom' to use setCustomShaders() function";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	m_vtxShdPath = vtxShdPath;
	m_frgShdPath = frgShdPath;

	compile(vtxShdPath, frgShdPath);
}

void Shader::setLight()
{
	compile("resources/shaders/unicolor.vs", "resources/shaders/unicolor.fs");
}

void Shader::setColor(RGBColor color)
{
	if (m_shaderType != ShaderType::Unicolor)
	{
		throw std::invalid_argument("Shader type must be 'unicolor' to use setColor() function");
	}

	m_color = color;
	compile("resources/shaders/unicolor.vs", "resources/shaders/unicolor.fs");
}

void Shader::setColor(std::string color)
{
	if (m_shaderType != ShaderType::Unicolor)
	{
		throw std::invalid_argument("Shader type must be 'unicolor' to use setColor() function");
	}

	// Basic string parsing test

	if (color.size() != 7 || color[0] != '#')
	{
		std::string err_msg = "Hex color is between #000000 and #ffffff";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	try
	{
		m_color.x = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0;
		m_color.y = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0;
		m_color.z = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0;
	} catch (...)
	{
		std::string err_msg = "Color hex code has unvalid format and must be #abc0123";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	compile("resources/shaders/unicolor.vs", "resources/shaders/unicolor.fs");
}

void Shader::addTexture(const std::filesystem::path& texturePath)
{
	if (m_shaderType != ShaderType::Texture)
	{
		throw std::logic_error("Shader type must be 'texture' to use setTexture() function");
	}

	if (!std::filesystem::exists(texturePath))
	{
		throw std::invalid_argument("Texture [" + texturePath.string() + "] doesn't not exist");
	}

	int width, height, nrChannels;
	if (m_font)
	{
		stbi_set_flip_vertically_on_load(false);
	}
	unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels, 0);

	if (!data)
	{
		throw std::runtime_error("Texture [" + texturePath.string() + "] failed to load");
	}

	m_texturesInfo.push_back({data, width, height, nrChannels, 0, texturePath});

	if (m_font)
	{
		stbi_set_flip_vertically_on_load(true);
		compile("resources/shaders/font.vs", "resources/shaders/font.fs");
	}
	else
		compile("resources/shaders/texture.vs", "resources/shaders/texture.fs");
}

void Shader::setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath)
{
	int success;
	unsigned int vtxShd = 0, frgShd = 0;
	m_name = vtxShdPath.string();

	// VERTEX SHADER

	const std::string vertexShaderContent = getFileContent(vtxShdPath);
	if (vertexShaderContent.empty())
	{
		throw std::invalid_argument("Vertex shader [" + vtxShdPath.string() + "]content could not be loaded");
	}

	const char* vertexShaderContentChar = vertexShaderContent.c_str();
	vtxShd = g_opengl.glCreateShader(GL_VERTEX_SHADER);
	g_opengl.glShaderSource(vtxShd, 1, &vertexShaderContentChar, NULL);
	g_opengl.glCompileShader(vtxShd);

	g_opengl.glGetShaderiv(vtxShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		g_opengl.glGetShaderInfoLog(vtxShd, INFO_LOG_SIZE, NULL, g_infoLog);
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument("Error compiling vertex shader: " + std::string(g_infoLog));
	}

	// FRAGMENT SHADER

	const std::string fragmentShaderContent = getFileContent(frgShdPath);
	if (fragmentShaderContent.empty())
	{
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument("Fragment shader [" + frgShdPath.string() + "] content could not be loaded");
	}
	const char* fragmentShaderContentChar = fragmentShaderContent.c_str();

	frgShd = g_opengl.glCreateShader(GL_FRAGMENT_SHADER);
	g_opengl.glShaderSource(frgShd, 1, &fragmentShaderContentChar, NULL);
	g_opengl.glCompileShader(frgShd);

	g_opengl.glGetShaderiv(frgShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		g_opengl.glGetShaderInfoLog(frgShd, INFO_LOG_SIZE, NULL, g_infoLog);
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument("Error compiling fragment shader: " + std::string(g_infoLog));
	}

	// SHADER LINKING

	m_shdPrgId = g_opengl.glCreateProgram();
	g_opengl.glAttachShader(m_shdPrgId, vtxShd);
	g_opengl.glAttachShader(m_shdPrgId, frgShd);
	g_opengl.glLinkProgram(m_shdPrgId);
	g_opengl.glGetProgramiv(m_shdPrgId, GL_LINK_STATUS, &success);

	deleteShaders(vtxShd, frgShd);
	if (success == GL_FALSE)
	{
		g_opengl.glGetProgramInfoLog(m_shdPrgId, INFO_LOG_SIZE, NULL, g_infoLog);
		throw std::invalid_argument("Error linking the shaders to the program: " + std::string(g_infoLog));
	}
}

void Shader::setTransformation(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	unsigned int modelUni = getUniform("t.model");
	g_opengl.glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewUni = getUniform("t.view");
	g_opengl.glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projectionUni = getUniform("t.projection");
	g_opengl.glUniformMatrix4fv(projectionUni, 1, GL_FALSE, glm::value_ptr(projection));
}

void Shader::use() const
{
	GLint isValid = GL_FALSE;
	g_opengl.glValidateProgram(m_shdPrgId);
	g_opengl.glGetProgramiv(m_shdPrgId, GL_VALIDATE_STATUS, &isValid);
	if (isValid == GL_FALSE)
	{
		std::string err_msg = "Program ID " + std::to_string(m_shdPrgId) + " is unvalid";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	g_opengl.glUseProgram(m_shdPrgId);

	switch (m_shaderType)
	{
		case ShaderType::Texture: {
			int uniform = 0;
			if (m_font)
				uniform = getUniform("atlas");
			else
				uniform = getUniform("material.diffuse_1");
			g_opengl.glUniform1i(uniform, 0);
			g_opengl.glActiveTexture(GL_TEXTURE0);
			g_opengl.glBindTexture(GL_TEXTURE_2D, m_texturesInfo[0].buffer);

			if (m_font)
				break;

			uniform = getUniform("material.specular_1");
			g_opengl.glUniform1i(uniform, 1);
			g_opengl.glActiveTexture(GL_TEXTURE1);
			g_opengl.glBindTexture(GL_TEXTURE_2D, m_texturesInfo[1].buffer);

			uniform = getUniform("material.shininess");
			g_opengl.glUniform1f(uniform, 32);
		}
		break;
		case ShaderType::Unicolor: {
			int uniform = getUniform("outColor");
			g_opengl.glUniform3f(uniform, m_color.x, m_color.y, m_color.z);
		}
		break;
		case ShaderType::Light: {
			int uniform = getUniform("outColor");
			g_opengl.glUniform3f(uniform, 1.0, 1.0, 1.0);
		}
		break;
		case ShaderType::Custom:
		default:
			break;
	}
}

std::string Shader::getFileContent(const std::filesystem::path& path)
{

	if (!std::filesystem::exists(path))
	{
		qCritical() << "Path [" << path.string() << "] doesn't not exist";
		return "";
	}

	std::ifstream file(path);
	if (!file)
	{
		qCritical() << "Path [" << path.string() << "] could not be opened";
		return "";
	}

	std::ostringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

void Shader::deleteShaders(unsigned int vtx, unsigned frg)
{
	if (vtx)
		g_opengl.glDeleteShader(vtx);
	if (frg)
		g_opengl.glDeleteShader(frg);
}

void Shader::deleteTexture()
{
	g_opengl.glBindTexture(GL_TEXTURE_2D, 0);
	g_opengl.glDeleteTextures(1, &m_texturesInfo.back().buffer);
	m_texturesInfo.back().buffer = 0;
}

void Shader::deleteProgram()
{
	g_opengl.glUseProgram(0);
	g_opengl.glDeleteProgram(m_shdPrgId);
	m_shdPrgId = 0;
}

int Shader::getUniform(std::string uniform) const
{
	int ret = g_opengl.glGetUniformLocation(m_shdPrgId, uniform.c_str());
	if (ret == -1)
	{
		throw std::runtime_error("Uniform [" + uniform + "] doesn't exist");
	}
	return ret;
}

void Shader::compile(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath)
{
	if (vtxShdPath.empty() || frgShdPath.empty())
	{
		throw std::invalid_argument("Vertex and fragment shaders have not been specified before compilation");
	}

	switch (m_shaderType)
	{
		case ShaderType::Texture: {
			// No need to set the shaders if a texture has already been added
			if (m_texturesInfo.size() > 1)
			{
				return;
			}

			TextureInfo& textInfo = m_texturesInfo.back();
			if (!textInfo.data)
			{
				throw std::invalid_argument("Texture data have not been loaded before compilation");
			}

			GLenum format;

			switch (textInfo.nrChannels)
			{
				case 1:
					format = GL_RED;
					break;
				case 3:
					format = GL_RGB;
					break;
				case 4:
					format = GL_RGBA;
					break;
				default:
					throw std::runtime_error("Unknown texture type");
					break;
			}

			if (m_font)
				g_opengl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			g_opengl.glGenTextures(1, &textInfo.buffer);
			g_opengl.glBindTexture(GL_TEXTURE_2D, textInfo.buffer);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, format, textInfo.width, textInfo.height, 0, format,
								  GL_UNSIGNED_BYTE, textInfo.data);
			g_opengl.glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(textInfo.data);
		}
		break;
		default:
			break;
	}

	setShaders(vtxShdPath, frgShdPath);
}

int TextureFromFile(const std::filesystem::path& texturePath, TextureType type)
{
	if (g_loadedTextures.find(texturePath) != g_loadedTextures.end())
	{
		return g_loadedTextures[texturePath].oglID;
	}

	if (!std::filesystem::exists(texturePath))
	{
		throw std::invalid_argument("Texture [" + texturePath.string() + "] doesn't not exist");
	}

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels, 0);

	if (!data)
	{
		throw std::runtime_error("Texture [" + texturePath.string() + "] failed to load");
	}

	GLenum format;

	switch (nrChannels)
	{
		case 1:
			format = GL_RED;
			break;
		case 3:
			format = GL_RGB;
			break;
		case 4:
			format = GL_RGBA;
			break;
		default:
			throw std::runtime_error("Unknown texture type");
			break;
	}

	unsigned int buffer;
	g_opengl.glGenTextures(1, &buffer);
	g_opengl.glBindTexture(GL_TEXTURE_2D, buffer);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	g_opengl.glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	g_opengl.glBindTexture(GL_TEXTURE_2D, 0);
	g_loadedTextures[texturePath] = {buffer, type};

	return buffer;
}

ShaderType Shader::getType(void) const
{
	return m_shaderType;
}