#include "Vertex.hpp"

Vertex::Vertex(SpaceCoord loc) : m_spaceCoord{ loc } {
}

std::vector<float> Vertex::getSpaceCoord() const {
	return { m_spaceCoord.x, m_spaceCoord.y, m_spaceCoord.z };
}

void Vertex::setSpaceCoord(SpaceCoord coord) {
	m_spaceCoord = coord;
}

std::vector<float> Vertex::getTextCoord() const {
	return { m_textCoord.x, m_textCoord.y };
}

void Vertex::setTextureCoordinates(const TextCoord &textCoord) {
	m_textCoord = textCoord;
}
