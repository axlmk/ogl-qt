#pragma once

#include <QOpenGLWindow>
#include <QTimer>
#include <unordered_map>

#include "glm/glm.hpp"

class Scene;

/**
 * @brief The view of the 3D scene as Qt Window
 */
class SceneViewer : public QOpenGLWindow
{
	Q_OBJECT
   public:
	/**
	 * @brief Constructor
	 * @param[in] scene The scene to represent
	 */
	SceneViewer(Scene* scene);

	/**
	 * @brief Destructor
	 */
	~SceneViewer() Q_DECL_OVERRIDE;

	/**
	 * @brief Initialize OpenGL properties
	 */
	void initializeGL() Q_DECL_OVERRIDE;

	/**
	 * @brief Called when the window is changing size or monitor
	 * @param[in] w The new width of the window
	 * @param[in] h The new height of the window
	 */
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	/**
	 * @brief Render an OpenGL frame
	 */
	void paintGL() Q_DECL_OVERRIDE;

	/**
	 * @brief Get the mouse pressed event
	 * @param[in] event The event captured
	 */
	void mousePressEvent(QMouseEvent* event) override;

	/**
	 * @brief Get the mouse released event
	 * @param[in] event The event captured
	 */
	void mouseReleaseEvent(QMouseEvent* event) override;

	/**
	 * @brief Get the mouse moved event
	 * @param[in] event The event captured
	 */
	void mouseMoveEvent(QMouseEvent* event) override;

	/**
	 * @brief Get the key pressed event
	 * @param[in] event The event captured
	 */
	void keyPressEvent(QKeyEvent* event) override;

	/**
	 * @brief Get the key released event
	 * @param[in] event The event captured
	 */
	void keyReleaseEvent(QKeyEvent* event) override;

   signals:
	/**
	 * @brief Indicates the OpenGL is initialized
	 */
	void initialized(void);

   private:
	Scene* m_manager;  ///< The scene that is represented by this SceneViewer

	std::unordered_map<std::string, bool> m_inputsBeingPressed;	 ///< Store all the keys currently pressed

	glm::ivec2 m_lastMousePos;		 ///< The last mouse position recorded
	glm::ivec2 m_lastFrameMousePos;	 ///< The mouse position recorded during last frame

	qint64 m_currentTime;			  ///< The last time a frame has been rendered
	qint64 m_deltaTime;				  ///< The time difference between two renders
	std::unique_ptr<QTimer> m_timer;  ///< A timer used to periodically render the scene
};