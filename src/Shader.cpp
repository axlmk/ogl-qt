#include <Shader.hpp>

Shader::Shader() : m_shdPrgId{}, m_shaderType{}, m_txtBuff{}, m_font{ false } {
}



Shader::Shader(ShaderType shaderType) : Shader() {
	m_shaderType = shaderType;
}





Shader::~Shader()
{
	if(m_shdPrgId)
	{
		g_opengl.glDeleteProgram(m_shdPrgId);
		m_shdPrgId = 0;
	}
}



void Shader::setShaderType(const ShaderType &shaderType) {
	m_shaderType = shaderType;
}



ShaderType Shader::getShaderType() const {
	return m_shaderType;
}



void Shader::setCustom(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath) {
	if (m_shaderType != ShaderType::Custom) {
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
	compile("shaders/unicolor.vs", "shaders/light.fs");
}


void Shader::setColor(RGBColor color)
{
	if (m_shaderType != ShaderType::Unicolor) {
		std::string err_msg = "Shader type must be 'unicolor' to use setColor() function";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	if(m_txtBuff != 0) {
		deleteTexture();
		deleteProgram();
	}

	m_color = color;
	compile("shaders/unicolor.vs", "shaders/unicolor.fs");
}



void Shader::setColor(std::string color)
{	
	if (m_shaderType != ShaderType::Unicolor) {
		std::string err_msg = "Shader type must be 'unicolor' to use setColor() function";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	if (m_txtBuff != 0) {
		deleteTexture();
		deleteProgram();
	}

	// Basic string parsing test

	if(color.size() != 7 || color[0] != '#') {
		std::string err_msg = "Hex color is between #000000 and #ffffff";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	try {
		m_color.x = std::stoi(color.substr(1, 2), nullptr, 16) / 255.0;
		m_color.y = std::stoi(color.substr(3, 2), nullptr, 16) / 255.0;
		m_color.z = std::stoi(color.substr(5, 2), nullptr, 16) / 255.0;
	} catch (...) {
		std::string err_msg = "Color hex code has unvalid format and must be #abc0123";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
	
	compile("shaders/unicolor.vs", "shaders/unicolor.fs");
}



void Shader::setTexture(const std::filesystem::path &texturePath, bool isFont) {
	if (m_shaderType != ShaderType::Texture) {
		std::string err_msg = "Shader type must be 'texture' to use setTexture() function";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	if (!std::filesystem::exists(texturePath)) {
		std::string err_msg = "Path[" + texturePath.string() + "] doesn't not exist";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels, 0);

	if (!data) {
		std::string err_msg = "Texture located at '" + texturePath.string() + "' failed to load";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	m_textureInfo.data = data;
	m_textureInfo.width = width;
	m_textureInfo.height = height;
	m_textureInfo.nrChannels = nrChannels;

	m_font = isFont;

	if(m_font)
		compile("shaders/font.vs", "shaders/font.fs");
	else
		compile("shaders/texture.vs", "shaders/texture.fs");
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
		std::string err_msg = "Vertex shader content could not be loaded";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
	
	const char* vertexShaderContentChar = vertexShaderContent.c_str();
	vtxShd = g_opengl.glCreateShader(GL_VERTEX_SHADER);
	g_opengl.glShaderSource(vtxShd, 1, &vertexShaderContentChar, NULL);
	g_opengl.glCompileShader(vtxShd);

	g_opengl.glGetShaderiv(vtxShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		g_opengl.glGetShaderInfoLog(vtxShd, INFO_LOG_SIZE, NULL, g_infoLog);
		std::string err_msg = "Error compiling vertex shader: " + std::string(g_infoLog);
		qCritical() << err_msg;
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument(err_msg);
	}

	// FRAGMENT SHADER

	const std::string fragmentShaderContent = getFileContent(frgShdPath);
	if (fragmentShaderContent.empty())
	{
		std::string err_msg = "Fragment shader content could not be loaded";
		qCritical() << err_msg;
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument(err_msg);
	}
	const char* fragmentShaderContentChar = fragmentShaderContent.c_str();

	frgShd = g_opengl.glCreateShader(GL_FRAGMENT_SHADER);
	g_opengl.glShaderSource(frgShd, 1, &fragmentShaderContentChar, NULL);
	g_opengl.glCompileShader(frgShd);
	
	g_opengl.glGetShaderiv(frgShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		g_opengl.glGetShaderInfoLog(frgShd, INFO_LOG_SIZE, NULL, g_infoLog);
		std::string err_msg = "Error compiling fragment shader: " + std::string(g_infoLog);
		qCritical() << err_msg;
		deleteShaders(vtxShd, frgShd);
		throw std::invalid_argument(err_msg);
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
		std::string err_msg = "Error linking the shaders to the program: " + std::string(g_infoLog);
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
}



void Shader::setTransformation(glm::mat4 model, glm::mat4 view, glm::mat4 projection)
{
	unsigned int modelUni = getUniform("model");
	g_opengl.glUniformMatrix4fv(modelUni, 1, GL_FALSE, glm::value_ptr(model));

	unsigned int viewUni = getUniform("view");
	g_opengl.glUniformMatrix4fv(viewUni, 1, GL_FALSE, glm::value_ptr(view));

	unsigned int projectionUni = getUniform("projection");
	g_opengl.glUniformMatrix4fv(projectionUni, 1, GL_FALSE, glm::value_ptr(projection));
}



void Shader::use() const
{
	GLint isValid = GL_FALSE;
	
	g_opengl.glValidateProgram(m_shdPrgId);
	g_opengl.glGetProgramiv(m_shdPrgId, GL_VALIDATE_STATUS, &isValid);
	if (isValid == GL_FALSE) {
		std::string err_msg = "Program ID " + std::to_string(m_shdPrgId) + " is unvalid";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	if(m_shaderType == ShaderType::Texture)
		g_opengl.glBindTexture(GL_TEXTURE_2D, m_txtBuff);

	g_opengl.glUseProgram(m_shdPrgId);

	if(m_shaderType == ShaderType::Light)
	{
		int uniform = getUniform("color");
		g_opengl.glUniform3f(uniform, 1.0, 1.0, 1.0);
	}
	else if (m_shaderType == ShaderType::Unicolor)
	{
		int uniform = getUniform("material.ambient");
		g_opengl.glUniform3f(uniform, 1.0f, 0.5f, 0.31f);

		uniform = getUniform("material.diffuse");
		g_opengl.glUniform3f(uniform, 1.0f, 0.5f, 0.31f);

		uniform = getUniform("material.specular");
		g_opengl.glUniform3f(uniform, 0.5f, 0.5f, 0.5f);

		uniform = getUniform("material.shininess");
		g_opengl.glUniform1f(uniform, 32.0f);
	}
}



std::string Shader::getFileContent(const std::filesystem::path &path) {

	if (!std::filesystem::exists(path)) {
		qCritical() << "Path [" << path.string() << "] doesn't not exist";
		return "";
	}

	std::ifstream file(path);
	if (!file) {
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
	if(vtx)
		g_opengl.glDeleteShader(vtx);
	if(frg)
		g_opengl.glDeleteShader(frg);
}



void Shader::deleteTexture() {
	g_opengl.glBindTexture(GL_TEXTURE_2D, 0);
	g_opengl.glDeleteTextures(1, &m_txtBuff);
	m_txtBuff = 0;
}



void Shader::deleteProgram() {
	g_opengl.glUseProgram(0);
	g_opengl.glDeleteProgram(m_shdPrgId);
	m_shdPrgId = 0;
}


int Shader::getUniform(std::string uniform) const
{
	int ret = g_opengl.glGetUniformLocation(m_shdPrgId, uniform.c_str());
	if(ret == -1)
	{
		std::string err_msg = "Uniform [" + uniform + "] doesn't exist";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
	return ret;
}


void Shader::compile(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath)
{
	switch (m_shaderType) {
		case ShaderType::Texture:
			if (!m_textureInfo.data) {
				std::string err_msg = "Texture data have not been loaded before compilation";
				qCritical() << err_msg;
				throw std::invalid_argument(err_msg);
			}

			GLenum format;

			switch (m_textureInfo.nrChannels) {
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
					throw std::invalid_argument("Unknown texture type");
					break;
			}

			if (m_font)
				g_opengl.glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			g_opengl.glGenTextures(1, &m_txtBuff);
			g_opengl.glBindTexture(GL_TEXTURE_2D, m_txtBuff);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, format, m_textureInfo.width, m_textureInfo.height, 0, format, GL_UNSIGNED_BYTE, m_textureInfo.data);
			g_opengl.glGenerateMipmap(GL_TEXTURE_2D);

			stbi_image_free(m_textureInfo.data);

			break;
		case ShaderType::Unicolor:
		case ShaderType::Light:
		case ShaderType::Custom:
		default:
			break;
	}

	if (vtxShdPath.empty() || frgShdPath.empty()) {
		std::string err_msg = "Vertex and fragment shaders have not been specified before compilation";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	setShaders(vtxShdPath, frgShdPath);
}