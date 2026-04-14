#include "PickingTexture.hpp"

PickingTexture::PickingTexture(int windowWidth, int windowHeight)
{
	g_opengl.glGenFramebuffers(1, &m_fbo);
	g_opengl.glGenTextures(1, &m_pickingTex);
	g_opengl.glGenTextures(1, &m_depthTex);

	_updateTexture(windowWidth, windowHeight);
}

void PickingTexture::updateTexture(int windowWidth, int windowHeight)
{
	_updateTexture(windowWidth, windowHeight);
}

void PickingTexture::_updateTexture(int windowWidth, int windowHeight)
{
	g_opengl.glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

	g_opengl.glBindTexture(GL_TEXTURE_2D, m_pickingTex);
	g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	g_opengl.glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	g_opengl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTex, 0);

	g_opengl.glBindTexture(GL_TEXTURE_2D, m_depthTex);
	g_opengl.glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowWidth, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	g_opengl.glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTex, 0);

	GLenum status = g_opengl.glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		qDebug() << "Erreur chef" << status;
		exit(1);
	}

	g_opengl.glBindFramebuffer(GL_FRAMEBUFFER, 0);
	g_opengl.glBindTexture(GL_TEXTURE_2D, 0);
	m_windowDim.x = windowWidth;
	m_windowDim.y = windowHeight;
}

void PickingTexture::enableWriting()
{
	g_opengl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo);
}

void PickingTexture::disableWriting()
{
	g_opengl.glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

glm::ivec3 PickingTexture::readPixel(uint mouseX, uint mouseY) const
{
	unsigned char color[3];
	g_opengl.glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo);
	g_opengl.glReadBuffer(GL_COLOR_ATTACHMENT0);
	g_opengl.glFlush();
	g_opengl.glReadPixels(mouseX, static_cast<int>(m_windowDim.y) - mouseY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, color);
	g_opengl.glReadBuffer(GL_NONE);
	g_opengl.glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	return {static_cast<int>(color[0]), static_cast<int>(color[1]), static_cast<int>(color[2])};
}