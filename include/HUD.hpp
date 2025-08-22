#pragma once

#include <algorithm>

#include <QTDebug>
#include <ft2build.h>
#include <glm/glm.hpp>
#include "Utils.hpp"
#include <stb_image_write.h>

#include "Shader.hpp"

#include FT_FREETYPE_H

#define ATLAS_WIDTH     330
#define ATLAS_HEIGHT    330

struct Character {
    glm::ivec2   size;      // Size of glyph
    glm::ivec2   bearing;   // Offset from baseline to left/top of glyph
    glm::vec2    atlasPos;  // top left
    unsigned int advance;   // Offset to advance to next glyph
    std::vector<unsigned char> bitmap;
};

class HUD {
public:

	HUD(std::string font);
    ~HUD();
    void RenderText(float x, float y, glm::vec3 color, float scale = 1);
    void setText(std::string text);
    void setText(const std::function<std::string(void)>& text);

private:
    void generateCharacters(FT_Face face);
    void generateAtlas(std::string font);
    void generateOGLBuffers();

    std::map<char, Character> m_characters;

    std::string m_text = "Default";
    std::function<std::string(void)> m_mutableText;
    bool m_isMutable = false;

    unsigned int m_vao;
    unsigned int m_vbo;
    std::unique_ptr<Shader> m_shd;
};