#pragma once
#include <array>
#include <vector>
#include <QDebug>
#include <exception>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
	

	void setTextureMapping();
	void unsetTextureMapping();

	void translate(float x = 0.f, float y = 0.f, float z = 0.f);
	void scale(float s = 1.f);
	void rotate(float angle = 0.f, float x = 0.f, float y = 0.f, float z = 0.f);
	glm::mat4 getTransformation(bool reset);

	bool empty() const;

private:
	GeometryType m_geoType;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_verticesLink;
	std::array<unsigned int, VERTEX_NUMBER_OF_DATA> m_attributesPositions;

	glm::mat4 m_transformation;
	
	bool m_hasTexture;
	uint getStrideLength() const;

};