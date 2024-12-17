#include <Shader.hpp>

Shader::Shader() : m_shdPrgId{}, m_vtxShd{}, m_frgShd{} {
}

Shader::Shader(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath) {
	addShaders(vtxShdPath, frgShdPath);
}

Shader::~Shader() {
	if(m_shdPrgId)
		g_opengl.glDeleteProgram(m_shdPrgId);
}

void Shader::addShaders(const std::filesystem::path& vtxShdPath, const std::filesystem::path& frgShdPath) {
	
	int success;
	/* Verter shader management */

	const std::string vertexShaderContent = getFileContent(vtxShdPath);
	if (vertexShaderContent.empty()) {
		m_err_msg = "Vertex shader content could not be loaded";
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
	const char* vertexShaderContentChar = vertexShaderContent.c_str();
	m_vtxShd = g_opengl.glCreateShader(GL_VERTEX_SHADER); // Create new shader id
	g_opengl.glShaderSource(m_vtxShd, 1, &vertexShaderContentChar, NULL); // Load the content of the shader into an opengl shader
	g_opengl.glCompileShader(m_vtxShd); // Compile it
	g_opengl.glGetShaderiv(m_vtxShd, GL_COMPILE_STATUS, &success); // Get debug info if operation failed

	if (!success) {
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
	if (!success) {
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
	if (!success) {
		g_opengl.glGetProgramInfoLog(m_shdPrgId, INFO_LOG_SIZE, NULL, g_infoLog);
		m_err_msg = "Error linking the shaders to the program: " + std::string(g_infoLog);
		qCritical() << m_err_msg;
		throw std::invalid_argument(m_err_msg);
	}
}

void Shader::use() const {
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