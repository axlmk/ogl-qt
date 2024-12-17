#include "RectangleWindow.hpp"

RectangleWindow::~RectangleWindow() {

}

void RectangleWindow::initializeGL() {

	/* Initialize OpenGL */

	try {
		g_opengl.initializeOpenGLFunctions();
	} catch (...) {
		qCritical() << "OpenGL could not be initialized";
	}

	/* Initialization of debug variables */

	int success = 0;

	/* 3D Model creation */

	float renderedObject[] = {
		// positions        // colors         // m_texture
		 -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		 -0.5f,-0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		  0.5f,-0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		1, 2, 3
	};

	/* Vertex Array Object (VAO) management */
	
	g_opengl.glGenVertexArrays(1, &m_VAO);
	g_opengl.glBindVertexArray(m_VAO);

	/* Element Buffer Object (EBO) management */
	
	unsigned int m_EBO;
	g_opengl.glGenBuffers(1, &m_EBO);
	g_opengl.glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	g_opengl.glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	/* Vertex Buffer Object (VBO) management */

	unsigned int VBO; // Vertex buffer object, used to manage vertex data in the GPU memory
	g_opengl.glGenBuffers(1, &VBO); // generate random buffer
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, VBO); // make VBO a buffer of type GL_ARRAY_BUFFER
	// glBufferData => copy the rendered object into the vertex buffer object thanks to the GL_ARRAY_BUFFER target
	g_opengl.glBufferData(GL_ARRAY_BUFFER, sizeof(renderedObject), renderedObject, GL_STATIC_DRAW);
	// Tells OpenGL how to interpret the raw data
	g_opengl.glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	g_opengl.glEnableVertexAttribArray(0);
	g_opengl.glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	g_opengl.glEnableVertexAttribArray(1);
	g_opengl.glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float)));
	g_opengl.glEnableVertexAttribArray(2);
	
	// Right now we sent the input vertex data to the GPU 
	// and instructed the GPU how it should process the vertex data within a vertex and fragment shader.

	m_shd.addShaders("shaders/pass_through.vert", "shaders/uniform_color.frag");
	unsigned int m_texture;
	g_opengl.glGenTextures(1, &m_texture);
	g_opengl.glBindTexture(GL_TEXTURE_2D, m_texture);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("textures/container.jpg", &width, &height, &nrChannels, 0);

	if(data) {
		g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		g_opengl.glGenerateMipmap(GL_TEXTURE_2D);
	} else {
		qCritical() << "Texture couldn't be loaded";
		return;
	}

	stbi_image_free(data);

}



void RectangleWindow::paintGL() {
	g_opengl.glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	g_opengl.glClear(GL_COLOR_BUFFER_BIT);

	m_shd.use();

	g_opengl.glBindTexture(GL_TEXTURE_2D, m_texture);
	g_opengl.glBindVertexArray(m_VAO);
	g_opengl.glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	g_opengl.glBindVertexArray(0);
}
