#pragma once

#include <QOpenGLWindow>
#include <QTimer>
#include <unordered_map>

#include "glm/glm.hpp"

class scene;

class SceneViewer : public QOpenGLWindow
{
   public:
	SceneViewer(scene* scene);
	~SceneViewer() Q_DECL_OVERRIDE;

	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

   private:
	scene* m_manager;

	std::unordered_map<std::string, bool> m_inputsBeingPressed;

	glm::ivec2 m_lastMousePos;
	glm::ivec2 m_lastFrameMousePos;

	qint64 m_currentTime;
	qint64 m_deltaTime;
	std::unique_ptr<QTimer> m_timer;

	glm::vec3 mouse_coordinates;
};