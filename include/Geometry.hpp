#pragma once
#include <array>
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
	using uint = unsigned int;
	
	// TODO : rule of 3
	
	Geometry();
	Geometry(GeometryType geoType, SpaceCoord initialPos, float size);

	std::vector<Vertex>	getVertices()				const;
	std::vector<float>	getFloatVertices()			const;
	std::vector<uint>	getVerticesLink()			const;
	std::vector<uint>	getAttributesPositions()	const;

	bool empty() const;
	void setTextureMapping();
	void unsetTextureMapping();

private:
	GeometryType m_geoType;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_verticesLink;
	std::array<unsigned int, VERTEX_NUMBER_OF_DATA> m_attributesPositions;
	
	bool m_hasTexture;
	uint getStrideLength() const;

};