#ifndef TriangleWindow_H
#define TriangleWindow_H

#include "OpenGLWindow.hpp"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class TriangleWindow : public OpenGLWindow {
public:
	~TriangleWindow() Q_DECL_OVERRIDE;

	void initialize() Q_DECL_OVERRIDE;
	void render() Q_DECL_OVERRIDE;

private:
	QOpenGLBuffer m_vertexBufferObject;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLShaderProgram *m_program;
};

#endif