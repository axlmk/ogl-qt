#pragma once
#include <vector>
#include <QDebug>
#include <exception>

#include "Vertex.hpp"
#define VERTICE_LINK_SIZEOF sizeof(unsigned int)

enum class GeometryType {
	Cube,
	Square,
	Model
};

std::string toString(GeometryType geoType);

class Geometry {
public:
	
	Geometry(GeometryType geoType, Location initialPos, float size);

	std::vector<Vertex> getVertices() const;
	std::vector<unsigned int> getVerticesLink() const;
	std::vector<unsigned int> getAttributesPositions() const;

private:
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_verticesLink;
	std::vector<unsigned int> m_attributesPositions;
};