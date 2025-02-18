#include "Vertex.hpp"

Vertex::Vertex(SpaceCoord loc) : m_spaceCoord{ loc } {
}

std::vector<float> Vertex::getSpaceCoord() const {
	return { m_spaceCoord.x, m_spaceCoord.y, m_spaceCoord.z };
}

void Vertex::setSpaceCoord(SpaceCoord coord) {
	m_spaceCoord = coord;
}

std::vector<TextCoord> Vertex::getTextCoord() const {
	return { { m_textCoord.x, m_textCoord.y } };
}

void Vertex::setTextureCoordinates(const TextCoord &textCoord) {
	m_textCoord = textCoord;
}

std::vector<SpaceCoord> Vertex::getNormals() const
{
	return { m_normals[0], m_normals[1], m_normals[2] };
}

void Vertex::setNormals(const std::vector<SpaceCoord>& normal)
{
	m_normals.push_back(normal[0]);
	m_normals.push_back(normal[1]);
	m_normals.push_back(normal[2]);
}