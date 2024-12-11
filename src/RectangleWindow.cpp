#include "RectangleWindow.hpp"

RectangleWindow::~RectangleWindow() {

}

void RectangleWindow::initializeGL() {

	/* Initialize OpenGL */

	try {
		initializeOpenGLFunctions();
	} catch (...) {
		qCritical() << "OpenGL could not be initialized";
	}

	/* Initialization of debug variables */

	int success = 0;
	char infoLog[INFO_LOG_SIZE];

	/* 3D Model creation */

	float renderedObject[] = {
		 0.0f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
	};

	/* Vertex Array Object management */
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	/* VBO management */

	unsigned int vertexBufferObject; // Vertex buffer object, used to manage vertex data in the GPU memory
	glGenBuffers(1, &vertexBufferObject); // generate random buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject); // make vertexBufferObject a buffer of type GL_ARRAY_BUFFER
	// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	// GL_STATIC_DRAW : the data is set only once and used many times.
	// GL_DYNAMIC_DRAW : the data is changed a lot and used many times.
	// glBufferData => copy the rendered object into the vertex buffer object thanks to the GL_ARRAY_BUFFER target
	glBufferData(GL_ARRAY_BUFFER, sizeof(renderedObject), renderedObject, GL_STATIC_DRAW);
	// Tells OpenGL how to interpret the raw data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/* Verter shader management */

	const std::string vertexShaderContent = getFileContent("shaders/pass_through.vert"); // Get shader content from file
	if (vertexShaderContent.empty()) {
		qCritical() << "Vertex shader content could not be loaded";
		return;
	}
	const char *vertexShaderContentChar = vertexShaderContent.c_str();
	unsigned int vertexShaderId;
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER); // Create new shader id
	glShaderSource(vertexShaderId, 1, &vertexShaderContentChar, NULL); // Load the content of the shader into an actual shader
	glCompileShader(vertexShaderId); // Compile it
	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success); // Get debug info if operation failed
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, INFO_LOG_SIZE, NULL, infoLog);
		qCritical() << "Error compiling vertex shader: " << infoLog;
	}

	/* Fragment shader management */

	const std::string fragmentShaderContent = getFileContent("shaders/uniform_color.frag");
	if (fragmentShaderContent.empty()) {
		qCritical() << "Fragment shader content could not be loaded";
		return;
	}
	const char * fragmentShaderContentChar = fragmentShaderContent.c_str();
	unsigned int fragmentShaderId;
	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShaderContentChar, NULL);
	glCompileShader(fragmentShaderId);
	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShaderId, INFO_LOG_SIZE, NULL, infoLog);
		qCritical() << "Error compiling fragment shader: " << infoLog;
	}

	/* Shader linking management */
	
	m_shaderProgramId = glCreateProgram(); // Creation of the program
	glAttachShader(m_shaderProgramId, vertexShaderId); // Attach both shaders to the program
	glAttachShader(m_shaderProgramId, fragmentShaderId);
	glLinkProgram(m_shaderProgramId); // Creation of the link
	glGetProgramiv(m_shaderProgramId, GL_LINK_STATUS, &success); // Get info on how the link went
	if (!success) {
		glGetProgramInfoLog(m_shaderProgramId, INFO_LOG_SIZE, NULL, infoLog);
		qCritical() << "Error linking the shaders to the program: " << infoLog;
	}
	glDeleteShader(vertexShaderId); // The shader are no more required once the linking has been done
	glDeleteShader(fragmentShaderId);
	// Right now we sent the input vertex data to the GPU 
	// and instructed the GPU how it should process the vertex data within a vertex and fragment shader.
}

std::string RectangleWindow::getFileContent(std::filesystem::path path) {

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
	return buffer.str();
}

void RectangleWindow::paintGL() {
	glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_shaderProgramId);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
}
