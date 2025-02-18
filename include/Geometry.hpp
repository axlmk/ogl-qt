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
	
	Geometry();
	Geometry(GeometryType geoType, float size);

	std::vector<Vertex>	getVertices()				const;
	std::vector<float>	getFloatVertices()			const;
	std::vector<uint>	getVerticesLink()			const;
	std::vector<uint>	getAttributesPositions()	const;
	

	void setTextureMapping();
	void unsetTextureMapping();

	void setNormals();

	void translate(float x = 0.f, float y = 0.f, float z = 0.f);
	void scale(float scale = 1.f);
	void rotate(float angle = 0.f, float x = 0.f, float y = 0.f, float z = 0.f);
	void setPivot(float x = 0., float y = 0., float z = 0.);

	SpaceCoord getPosition() const;
	glm::vec4 getRotation() const;

	bool empty() const;

private:
	GeometryType m_geoType;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_verticesLink;
	std::array<unsigned int, VERTEX_NUMBER_OF_DATA> m_attributesPositions;

	SpaceCoord m_position;
	glm::vec4 m_rotation;
	float m_scale;
	
	bool m_hasTexture;
	bool m_hasNormals;
	uint getStrideLength() const;

};