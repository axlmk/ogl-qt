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
	m_geoType { GeometryType::Square}  {

}



Geometry::Geometry(GeometryType	geoType, SpaceCoord initialPos, float size) : Geometry() {
	m_geoType = geoType;
	switch (m_geoType) {
		case GeometryType::Square:
			m_vertices.push_back(Vertex({ initialPos.x, initialPos.y, 0 }));
			m_vertices.push_back(Vertex({ initialPos.x + size, initialPos.y, 0 }));
			m_vertices.push_back(Vertex({ initialPos.x, initialPos.y + size, 0 }));
			m_vertices.push_back(Vertex({ initialPos.x + size, initialPos.y + size, 0 }));
			m_verticesLink = {	0, 1, 2,
								1, 2, 3 };
			break;
		case GeometryType::Cube:
			/*m_vertices.push_back(Vertex(initialPos));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x + size, initialPos.y, initialPos.z)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x, initialPos.y + size, initialPos.z)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x, initialPos.y, initialPos.z + size)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x + size, initialPos.y + size, initialPos.z)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x + size, initialPos.y, initialPos.z + size)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x, initialPos.y + size, initialPos.z + size)));
			m_vertices.push_back(Vertex(SpaceCoord(initialPos.x + size, initialPos.y + size, initialPos.z + size)));
			break;*/
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