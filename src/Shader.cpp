#include <Shader.hpp>

Shader::Shader() : m_shdPrgId{}, m_vtxShd{}, m_frgShd{}, m_shaderType{}, m_txtBuff{} {
}



Shader::Shader(ShaderType shaderType) : Shader() {
	m_shaderType = shaderType;
}



Shader::~Shader() {
	if(m_shdPrgId) {
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
		m_err_msg = "Shader type must be 'custom' to use setCustomShaders() function";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
	setShaders(vtxShdPath, frgShdPath);
}



void Shader::setColor() {
	if (m_shaderType != ShaderType::Unicolor) {
		m_err_msg = "Shader type must be 'unicolor' to use setColor() function";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}

	if(m_txtBuff != 0) {
		deleteTexture();
		deleteProgram();
	}
	setShaders("shaders/unicolor.vs", "shaders/unicolor.fs");
}



void Shader::setTexture(const std::filesystem::path &texturePath) {
	if (m_shaderType != ShaderType::Texture) {
		m_err_msg = "Shader type must be 'texture' to use setTexture() function";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(texturePath.string().c_str(), &width, &height, &nrChannels, 0);

	if (!data) {
		m_err_msg = "Texture located at '" + texturePath.string() + "' failed to load";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}

	g_opengl.glGenTextures(1, &m_txtBuff);
	g_opengl.glBindTexture(GL_TEXTURE_2D, m_txtBuff);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	g_opengl.glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	setShaders("shaders/texture.vs", "shaders/texture.fs");
}



void Shader::setShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath) {
	
	int success;

	/* Verter shader management */

	const std::string vertexShaderContent = getFileContent(vtxShdPath);
	if (vertexShaderContent.empty()) {
		m_err_msg = "Vertex shader content could not be loaded";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
	
	const char* vertexShaderContentChar = vertexShaderContent.c_str();
	m_vtxShd = g_opengl.glCreateShader(GL_VERTEX_SHADER);
	g_opengl.glShaderSource(m_vtxShd, 1, &vertexShaderContentChar, NULL);
	g_opengl.glCompileShader(m_vtxShd);

	g_opengl.glGetShaderiv(m_vtxShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		g_opengl.glGetShaderInfoLog(m_vtxShd, INFO_LOG_SIZE, NULL, g_infoLog);
		m_err_msg = "Error compiling vertex shader: " + std::string(g_infoLog);
		qCritical() << m_err_msg;
		deleteShaders();
		throw std::invalid_argument(m_err_msg);
	}

	/* Fragment shader management */

	const std::string fragmentShaderContent = getFileContent(frgShdPath);
	if (fragmentShaderContent.empty()) {
		m_err_msg = "Fragment shader content could not be loaded";
		qCritical() << m_err_msg;
		deleteShaders();
		throw std::invalid_argument(m_err_msg);
	}
	const char* fragmentShaderContentChar = fragmentShaderContent.c_str();

	m_frgShd = g_opengl.glCreateShader(GL_FRAGMENT_SHADER);
	g_opengl.glShaderSource(m_frgShd, 1, &fragmentShaderContentChar, NULL);
	g_opengl.glCompileShader(m_frgShd);
	
	g_opengl.glGetShaderiv(m_frgShd, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		g_opengl.glGetShaderInfoLog(m_frgShd, INFO_LOG_SIZE, NULL, g_infoLog);
		m_err_msg = "Error compiling fragment shader: " + std::string(g_infoLog);
		qCritical() << m_err_msg;
		deleteShaders();
		throw std::invalid_argument(m_err_msg);
	}

	/* Shader linking management */

	m_shdPrgId = g_opengl.glCreateProgram(); // Creation of the program
	g_opengl.glAttachShader(m_shdPrgId, m_vtxShd); // Attach both shaders to the program
	g_opengl.glAttachShader(m_shdPrgId, m_frgShd);
	g_opengl.glLinkProgram(m_shdPrgId); // Creation of the link
	g_opengl.glGetProgramiv(m_shdPrgId, GL_LINK_STATUS, &success); // Get info on how the link went
	
	deleteShaders();
	if (success == GL_FALSE) {
		g_opengl.glGetProgramInfoLog(m_shdPrgId, INFO_LOG_SIZE, NULL, g_infoLog);
		m_err_msg = "Error linking the shaders to the program: " + std::string(g_infoLog);
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
}



void Shader::use() const {
	GLint isValid = GL_FALSE;
	
	g_opengl.glValidateProgram(m_shdPrgId);
	g_opengl.glGetProgramiv(m_shdPrgId, GL_VALIDATE_STATUS, &isValid);
	if (isValid == GL_FALSE) {
		qCritical() << "Program ID" << m_shdPrgId << "is unvalid";
	}

	if(m_shaderType == ShaderType::Texture)
		// TODO : test if binding went well
		g_opengl.glBindTexture(GL_TEXTURE_2D, m_txtBuff);
	g_opengl.glUseProgram(m_shdPrgId);
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



void Shader::deleteShaders() {
	g_opengl.glDeleteShader(m_vtxShd);
	m_vtxShd = 0;
	g_opengl.glDeleteShader(m_frgShd);
	m_frgShd = 0;
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