#include "HUD.hpp"

HUD::HUD(std::string font)
{
	FT_Library ft;
	FT_Face face;
	if (FT_Init_FreeType(&ft))
	{
		std::string err_msg = "FREETYPE: Could not init FreeType Library";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}

	if (FT_New_Face(ft, ("resources/font/" + font + ".ttf").c_str(), 0, &face))
	{
		std::string err_msg = "FREETYPE: Failed to load font " + font;
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
	FT_Set_Pixel_Sizes(face, 0, 48);

	generateCharacters(face);

	generateAtlas(font);

	generateOGLBuffers();

	Shader* s = new Shader(ShaderType::Texture, true);
	s->addTexture("resources/textures/arial.png");
	m_shd = std::unique_ptr<Shader>(s);

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

HUD::~HUD()
{
	g_opengl.glDeleteBuffers(1, &m_vbo);
	g_opengl.glDeleteVertexArrays(1, &m_vao);
}

void HUD::generateOGLBuffers()
{
	g_opengl.glGenVertexArrays(1, &m_vao);
	g_opengl.glGenBuffers(1, &m_vbo);

	g_opengl.glBindVertexArray(m_vao);
	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	g_opengl.glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	g_opengl.glEnableVertexAttribArray(0);
	g_opengl.glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

	g_opengl.glBindBuffer(GL_ARRAY_BUFFER, 0);
	g_opengl.glBindVertexArray(0);
}

void HUD::generateCharacters(FT_Face face)
{
	for (unsigned char c = 32; c < 128; c++)
	{
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::string err_msg = "FREETYTPE: Failed to load Glyph";
			qCritical() << err_msg;
			throw std::invalid_argument(err_msg);
		}

		Character character = {glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
							   glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
							   glm::ivec2(0, 0),
							   static_cast<unsigned int>(face->glyph->advance.x),
							   {}};

		character.bitmap.resize(character.size.x * character.size.y);
		for (int y = 0; y < character.size.y; ++y)
		{
			for (int x = 0; x < character.size.x; ++x)
			{
				character.bitmap[y * character.size.x + x] = face->glyph->bitmap.buffer[y * character.size.x + x];
			}
		}

		m_characters.insert(std::pair<char, Character>(c, character));
	}
}

void HUD::generateAtlas(std::string font)
{
	std::vector<unsigned char> atlas(ATLAS_WIDTH * ATLAS_HEIGHT, 0);

	unsigned int xOffset{}, yOffset{}, rowHeight{};

	for (unsigned char c = 32; c < 128; ++c)
	{
		Character& ch = m_characters[c];

		// Check if the character fits in the current row
		if (xOffset + ch.size.x > ATLAS_WIDTH)
		{
			xOffset = 0;
			yOffset += rowHeight;
			rowHeight = 0;
		}

		// Copy the character bitmap into the atlas at the calculated position
		ch.atlasPos = glm::vec2(xOffset, yOffset);
		for (int y = 0; y < ch.size.y; ++y)
		{
			for (int x = 0; x < ch.size.x; ++x)
			{
				atlas[(yOffset + y) * ATLAS_WIDTH + (xOffset + x)] = ch.bitmap[y * ch.size.x + x];
			}
		}

		// Update offsets
		xOffset += ch.size.x;
		rowHeight = std::max(rowHeight, static_cast<unsigned int>(ch.size.y));
	}

	// Save the final atlas image
	int res = stbi_write_png("resources/textures/arial.png", ATLAS_WIDTH, ATLAS_HEIGHT, 1, atlas.data(), ATLAS_WIDTH);
	if (!res)
	{
		std::string err_msg = "Bitmap texture [" + font + "] could not be saved";
		qCritical() << err_msg;
		throw std::invalid_argument(err_msg);
	}
}

void HUD::RenderText(float x, float y, glm::vec3 color, float scale)
{
	m_shd->use();
	glm::mat4 ortho = glm::ortho(0.0, 800.0, 0.0, 600.0);
	g_opengl.glUniformMatrix4fv(m_shd->getUniform("transformation"), 1, GL_FALSE, glm::value_ptr(ortho));

	g_opengl.glUniform3f(m_shd->getUniform("textColor"), color.x, color.y, color.z);
	g_opengl.glBindVertexArray(m_vao);

	std::string text{};
	if (m_isMutable)
	{
		text = m_mutableText();
	}
	else
	{
		text = m_text;
	}

	for (unsigned char c : text)
	{
		Character& ch = m_characters[c];
		float xpos = x + ch.bearing.x * scale + 50;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale + 50;
		float w = ch.size.x * scale;
		float h = ch.size.y * scale;

		float vertices[6][4] = {
			{xpos, ypos + h, ch.atlasPos.x / ATLAS_WIDTH, ch.atlasPos.y / ATLAS_HEIGHT},
			{xpos, ypos, ch.atlasPos.x / ATLAS_WIDTH, (ch.atlasPos.y + ch.size.y) / ATLAS_HEIGHT},
			{xpos + w, ypos, (ch.atlasPos.x + ch.size.x) / ATLAS_WIDTH, (ch.atlasPos.y + ch.size.y) / ATLAS_HEIGHT},
			{xpos, ypos + h, ch.atlasPos.x / ATLAS_WIDTH, ch.atlasPos.y / ATLAS_HEIGHT},
			{xpos + w, ypos, (ch.atlasPos.x + ch.size.x) / ATLAS_WIDTH, (ch.atlasPos.y + ch.size.y) / ATLAS_HEIGHT},
			{xpos + w, ypos + h, (ch.atlasPos.x + ch.size.x) / ATLAS_WIDTH, ch.atlasPos.y / ATLAS_HEIGHT}};

		g_opengl.glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		g_opengl.glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		g_opengl.glBindBuffer(GL_ARRAY_BUFFER, 0);

		g_opengl.glDrawArrays(GL_TRIANGLES, 0, 6);
		x += (ch.advance >> 6) * scale;
	}
	g_opengl.glBindVertexArray(0);
	g_opengl.glBindTexture(GL_TEXTURE_2D, 0);
}

void HUD::setText(std::string text)
{
	m_text = text;
	m_isMutable = false;
}

void HUD::setText(const std::function<std::string(void)>& text)
{
	m_mutableText = text;
	m_isMutable = true;
}