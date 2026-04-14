#pragma once
#include <glm/glm.hpp>

#include "Utils.hpp"

class PickingTexture
{

	using uint = unsigned int;

   public:
	PickingTexture() = default;
	PickingTexture(int windowWidth, int windowHeight);

	/**
	 * @brief Updates the dimensions of the texture in case the screen is resized
	 * @param[in] windowWidth The width of the new texture
	 * @param[in] windowHeight The height of the new texture
	 */
	void updateTexture(int windowWidth, int windowHeight);

	void enableWriting();
	void disableWriting();

	glm::ivec3 readPixel(uint mouseX, uint mouseY) const;

   private:
	/**
	 * @brief Internally updates the dimensions of the texture in case the screen is resized
	 * @param[in] windowWidth The width of the new texture
	 * @param[in] windowHeight The height of the new texture
	 */
	void _updateTexture(int windowWidth, int windowHeight);

	uint m_fbo;
	uint m_pickingTex;
	uint m_depthTex;
	glm::uvec2 m_windowDim;
};