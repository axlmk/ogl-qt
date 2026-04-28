#pragma once

#include <ft2build.h>
#include <stb_image_write.h>

#include <QtDebug>
#include <algorithm>
#include <glm/glm.hpp>

#include "Shader.hpp"
#include "Utils.hpp"

#include FT_FREETYPE_H

#define ATLAS_WIDTH 330
#define ATLAS_HEIGHT 330

/**
 * @brief Represents all the characteristics of a character
 */
struct Character
{
	glm::ivec2 size;					///< Size of glyph
	glm::ivec2 bearing;					///< Offset from baseline to left/top of glyph
	glm::vec2 atlasPos;					///< top left
	unsigned int advance;				///< Offset to advance to next glyph
	std::vector<unsigned char> bitmap;	///< The bitmap representing the character
};

/**
 * @class Represents a HUD, 2D that can be added as an overlay on top of the scene
 */
class HUD
{
   public:
	/**
	 * @brief Constructor
	 * @param font The font family used to render the text
     */
	HUD(std::string font);

	/**
	 * @brief Destructor
	 */
	~HUD();

	/**
	 * @brief Render the text in the scene
	 * @param[in] x The x position of the text
	 * @param[in] y The y position of the text
	 * @param[in] color The color used to render the text
	 * @param[in] scale The scale used to render the text
	 */
	void RenderText(float x, float y, glm::vec3 color, float scale = 1);

	/**
	 * @brief Set the text to render
	 * @param[in] text The new text
	 */
	void setText(const std::string& text);

	/**
	 * @brief Set the text as a function to be called every time the HUD is rendered
	 * @param[in] text The function binded as a text
	 */
	void setText(const std::function<std::string(void)>& text);

   private:
	/**
	 * @brief Generate the character to render
	 * @param[in] face The font to use
	 */
	void _generateCharacters(FT_Face face);

	/**
	 * @brief Generate the atlas of character based on the provided font
	 * @param[in] font The font for rendering the atlas
	 */
	void _generateAtlas(std::string font);

	/**
	 * @brief Generate some OpenGL buffers
	 */
	void _generateOGLBuffers(void);

	std::map<char, Character> m_characters;	 ///< Map a character to its displayable representation

	std::string m_text = "Default";					 ///< The text to display
	std::function<std::string(void)> m_mutableText;	 ///< Display the result of a function instead of a static text
	bool m_isMutable = false;						 ///< Indicates if the HUD displays a static text or the result of a function

	unsigned int m_vao;				///< OpenGL buffer
	unsigned int m_vbo;				///< OpenGL buffer
	std::unique_ptr<Shader> m_shd;	///< Shader used to render the HUD
};