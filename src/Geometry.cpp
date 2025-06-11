#include "Geometry.hpp"

std::string toString(GeometryType geoType) {
	switch (geoType) {
		case GeometryType::Cube:
			return "cube";
		case GeometryType::Square:
			return "square";
		case GeometryType::Model:
			return "model";
		default:
			return "undefined";
	}
}



Geometry::Geometry() : 
	m_hasTexture{ false },
	m_attributesPositions{ VERTEX_SPACE_COORD_SIZE, VERTEX_TEXT_COORD_SIZE },
	m_geoType { GeometryType::Square},
	m_position { glm::vec3(0.f) },
	m_rotation { glm::vec4(0.f, 1.f, 0.f, 0.f) } {

}



Geometry::Geometry(GeometryType	geoType, float size) : Geometry() {
	m_geoType = geoType;
	switch (m_geoType) {
		case GeometryType::Square:
			m_vertices.push_back(Vertex({ 0, 0, 0 }));
			m_vertices.push_back(Vertex({ size, 0, 0 }));
			m_vertices.push_back(Vertex({ 0, size, 0 }));
			m_vertices.push_back(Vertex({ size, size, 0 }));
			m_verticesLink = {	0, 1, 2,
								1, 2, 3 };
			break;
		case GeometryType::Cube:
			m_vertices.push_back(Vertex({ 0, 0, 0 }));
			m_vertices.push_back(Vertex({ size, 0, 0 }));
			m_vertices.push_back(Vertex({ 0, size, 0 }));
			m_vertices.push_back(Vertex({ size, size, 0 }));
			m_vertices.push_back(Vertex({ 0, 0, size }));
			m_vertices.push_back(Vertex({ size, 0, size }));
			m_vertices.push_back(Vertex({ 0, size, size }));
			m_vertices.push_back(Vertex({ size, size, size }));

			m_verticesLink = {	0, 1, 2, // 0
								1, 2, 3,
								2, 3, 6, // 1
								6, 7, 3,
								3, 1, 5, // 2
								3, 5, 7,
								2, 6, 4, // 3
								2, 0, 4,
								6, 7, 5, // 4
								4, 5, 6,
								0, 1, 4,
								1, 4, 5
			};
			break;
		case GeometryType::Model:
		default:
			std::string err_msg = "Geometry type [" + toString(geoType) + "] not implemented yet";
			qCritical() << err_msg;
			throw std::invalid_argument(err_msg);
	}
}



std::vector<Vertex> Geometry::getVertices() const {
	return m_vertices;
}



std::vector<float> Geometry::getFloatVertices() const {
	std::vector<float> floatVertices;
	floatVertices.reserve(m_vertices.size() * getStrideLength());

	for (size_t i = 0; i < m_vertices.size(); i++) {
		std::vector<float> tmpFloat = m_vertices[i].getSpaceCoord();
		std::copy(tmpFloat.begin(), tmpFloat.end(), std::back_inserter(floatVertices));

		if (m_hasTexture) {
			std::vector<float> tmpText = m_vertices[i].getTextCoord();
			std::copy(tmpText.begin(), tmpText.end(), std::back_inserter(floatVertices));
		}

	}
	return floatVertices;
}


std::vector<unsigned int> Geometry::getVerticesLink() const {
	return m_verticesLink;
}



std::vector<unsigned int> Geometry::getAttributesPositions() const {
	int n = 1;
	if(m_hasTexture)
		n++;
	return std::vector<unsigned int> (m_attributesPositions.begin(), m_attributesPositions.begin() + n);
}



unsigned int Geometry::getStrideLength() const {
	int len = VERTEX_SPACE_COORD_SIZE;
	if(m_hasTexture)
		len += VERTEX_TEXT_COORD_SIZE;
	return len;
}



void Geometry::translate(float x, float y, float z) {
	m_position = glm::vec3(x, y, z);
}

void Geometry::scale(float scale) {
	m_scale = scale;
}

void Geometry::rotate(float angle, float x, float y, float z) {
	m_rotation = glm::vec4(angle, x, y, z);
}



void Geometry::setPivot(float x, float y, float z) {
	for(auto &vertex : m_vertices) {
		auto coords = vertex.getSpaceCoord();
		vertex.setSpaceCoord(SpaceCoord(coords[0] - x, coords[1] - y, coords[2] - z));
	}
}



SpaceCoord Geometry::getPosition() const {
	return m_position;
}



glm::vec4 Geometry::getRotation() const {
	return m_rotation;
}


bool Geometry::empty() const {
	return m_vertices.empty();
}



void Geometry::setTextureMapping() {
	switch (m_geoType) {
		case GeometryType::Square:
			m_vertices[0].setTextureCoordinates({ 0.f, 0.f });
			m_vertices[1].setTextureCoordinates({ 1.f, 0.f });
			m_vertices[2].setTextureCoordinates({ 0.f, 1.f });
			m_vertices[3].setTextureCoordinates({ 1.f, 1.f });
			break;
		case GeometryType::Cube:
			m_vertices[0].setTextureCoordinates({ 0.f, 0.f });
			m_vertices[1].setTextureCoordinates({ 1.f, 0.f });
			m_vertices[2].setTextureCoordinates({ 0.f, 1.f });
			m_vertices[3].setTextureCoordinates({ 1.f, 1.f });
			m_vertices[4].setTextureCoordinates({ 0.f, 1.f });
			m_vertices[5].setTextureCoordinates({ 1.f, 1.f });
			m_vertices[6].setTextureCoordinates({ 0.f, 0.f });
			m_vertices[7].setTextureCoordinates({ 1.f, 0.f });
			break;
		case GeometryType::Model:
		default:
			std::string err_msg = "Geometry type [" + toString(m_geoType) + "] not implemented yet";
			qCritical() << err_msg;
			throw std::invalid_argument(err_msg);
	}
	m_hasTexture = true;
}

void Geometry::unsetTextureMapping() {
	m_hasTexture = false;
}