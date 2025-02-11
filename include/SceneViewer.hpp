#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>
#include <QApplication>

#include "Utils.hpp"
#include "SceneObject.hpp"

// Forward declaration
class scene;


class SceneViewer : public QOpenGLWindow {
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

	scene *m_manager;

	bool m_altBeingPressed;
	bool m_mousePressed[3];		// 0 = left, 1 = middle, 2 = right
	bool m_KeyBeingPressed[4];	// 0 = z, 1 = q, 2 = d, 3 = s
	
	glm::vec2 m_lastMousePos;
	glm::vec2 m_mousePosWhenPressed;

	qint64 m_currentTime;
	qint64 m_deltaTime;
	std::unique_ptr<QTimer> m_timer;
};