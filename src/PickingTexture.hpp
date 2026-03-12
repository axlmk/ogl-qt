#pragma once
#include <glm/glm.hpp>

#include "Utils.hpp"

class PickingTexture
{

	using uint = unsigned int;

   public:
	PickingTexture(int windowWidth, int windowHeight);

	void enableWriting();
	void disableWriting();

	glm::ivec3 readPixel(uint mouseX, uint mouseY) const;

   private:
	uint m_fbo;
	uint m_pickingTex;
	uint m_depthTex;
	glm::uvec2 m_windowDim;
};