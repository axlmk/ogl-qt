#pragma once
#include <vector>
#include <glm/glm.hpp>
#define VERTEX_NUMBER_OF_DATA 2 // Space coordinates + texture coordinates
#define VERTEX_SPACE_COORD_INDEX 0
#define VERTEX_SPACE_COORD_SIZE 3
#define VERTEX_TEXT_COORD_INDEX 3
#define VERTEX_TEXT_COORD_SIZE 2

using SpaceCoord = glm::vec3;
using TextCoord = glm::vec2;

class Vertex {
public:
	Vertex(SpaceCoord loc);

	std::vector<float> getSpaceCoord() const;
	std::vector<float> getTextCoord() const;

	void setTextureCoordinates(const TextCoord &textCoord);

private:
	SpaceCoord m_spaceCoord;
	TextCoord m_textCoord;
};