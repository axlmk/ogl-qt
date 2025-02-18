#pragma once
#include <vector>
#include <glm/glm.hpp>
#define VERTEX_NUMBER_OF_DATA 3 // Space coordinates + texture coordinates
#define VERTEX_SPACE_COORD_INDEX 0
#define VERTEX_SPACE_COORD_SIZE 3
#define VERTEX_TEXT_COORD_INDEX 3
#define VERTEX_TEXT_COORD_SIZE 2
#define VERTEX_NORMAL_SIZE 3

using SpaceCoord = glm::vec3;
using TextCoord = glm::vec2;

class Vertex {
public:
	Vertex(SpaceCoord loc);

	std::vector<float> getSpaceCoord() const;
	std::vector<TextCoord> getTextCoord() const;
	void setSpaceCoord(SpaceCoord coord);
	void setTextureCoordinates(const TextCoord &textCoord);

	std::vector<SpaceCoord> getNormals() const;
	void setNormals(const std::vector<SpaceCoord>& normal);


private:
	SpaceCoord m_spaceCoord;
	TextCoord m_textCoord;
	std::vector<SpaceCoord> m_normals;
};