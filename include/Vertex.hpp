#pragma once
#include <vector>
#include <glm/glm.hpp>
#define VERTEX_FULL_SIZE 3
#define VERTEX_LOCATION_INDEX 0
#define VERTEX_LOCATION_SIZE 3

using Location = glm::vec3;

class Vertex {
public:
	Vertex(Location loc);

	explicit operator std::vector<float>() const;

private:
	Location m_position;
};