#pragma once
#include <glm/glm.hpp>

#include "Utils.hpp"

/**
 * @brief Special texture used to select an object within the scene
 */
class PickingTexture
{

   public:
	/**
	 * @brief Default constructor
	 */
	PickingTexture() = default;

	/**
	 * @brief Constructor
	 * @param[in] windowWidth The width of the rendering window
	 * @param[in] windowHeight The height of the rendering window 
	 */
	PickingTexture(int windowWidth, int windowHeight);

	/**
	 * @brief Updates the dimensions of the texture in case the screen is resized
	 * @param[in] windowWidth The width of the new texture
	 * @param[in] windowHeight The height of the new texture
	 */
	void updateTexture(int windowWidth, int windowHeight);

	/**
	 * @brief Enable writing into the texture
	 */
	void enableWriting(void);

	/**
	 * @brief Disable writing into the texture
	 */
	void disableWriting(void);

	/**
	 * @brief Returns the color of a pixel at the location of the mouse
	 * @param[in] mouseX The X coordinate of the mouse
	 * @param[in] mouseY The Y coordinate of the mouse
	 * @return The color of the pixel under the mouse
	 */
	glm::ivec3 readPixel(uint mouseX, uint mouseY) const;

   private:
	/**
	 * @brief Internally updates the dimensions of the texture in case the screen is resized
	 * @param[in] windowWidth The width of the new texture
	 * @param[in] windowHeight The height of the new texture
	 */
	void _updateTexture(int windowWidth, int windowHeight);

	uint m_fbo;				 ///< OpenGL Frame Buffer Object
	uint m_pickingTex;		 ///< OpenGL texture id
	uint m_depthTex;		 ///< OpenGL depth texture id
	glm::uvec2 m_windowDim;	 ///< The dimensions of the rendering window
};