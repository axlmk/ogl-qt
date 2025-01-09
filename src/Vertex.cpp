#include "Vertex.hpp"

Vertex::Vertex(Location loc) {
	m_position = loc;
}

Vertex::operator std::vector<float>() const {
	std::vector<float> ret = {
		m_position.x, m_position.y, m_position.z
	};
	return ret;
}