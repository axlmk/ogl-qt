#ifndef RECTANGLE_WINDOW_HPP
#define RECTANGLE_WINDOW_HPP

#include <iostream>
#include <QOpenGLFunctions>
#include <QOpenGLWindow>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class RectangleWindow : public QOpenGLWindow, protected QOpenGLFunctions {
public:
	~RectangleWindow() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

private:
	QOpenGLBuffer m_indexBufferObject;
	QOpenGLBuffer m_vertexBufferObject;
	QOpenGLVertexArrayObject m_vao;
	QOpenGLShaderProgram *m_program;
};

#endif