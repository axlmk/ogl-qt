#pragma once

#include <QOpenGLExtraFunctions>
#include <QOpenGLWindow>
#include <QDebug>
#include <QKeyEvent>
#include <QTimer>
#include <QDateTime>

#include "Utils.hpp"
#include "SceneObject.hpp"
#include "SceneManager.hpp"

class SceneViewer : public QOpenGLWindow {
public:
	
	SceneViewer(std::shared_ptr<SceneManager> sceneManager);
	~SceneViewer() Q_DECL_OVERRIDE;


	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;

	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

private:

	std::shared_ptr<SceneManager> m_sceneManager;

	// 0 = z, 1 = q, 2 = d, 3 = s
	bool m_KeyBeingPressed[4];

	qint64 m_currentTime;
	qint64 m_deltaTime;
	std::unique_ptr<QTimer> m_timer;
};