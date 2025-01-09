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

Geometry::Geometry(GeometryType	geoType, Location initialPos, float size) {
	switch (geoType) {
		case GeometryType::Cube:
			m_vertices.push_back(Vertex(initialPos));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y, initialPos.z)));
			m_vertices.push_back(Vertex(Location(initialPos.x, initialPos.y + size, initialPos.z)));
			m_vertices.push_back(Vertex(Location(initialPos.x, initialPos.y, initialPos.z + size)));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y + size, initialPos.z)));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y, initialPos.z + size)));
			m_vertices.push_back(Vertex(Location(initialPos.x, initialPos.y + size, initialPos.z + size)));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y + size, initialPos.z + size)));

			break;
		case GeometryType::Square:
			m_vertices.push_back(Vertex(Location(initialPos.x, initialPos.y, 0)));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y, 0)));
			m_vertices.push_back(Vertex(Location(initialPos.x, initialPos.y + size, 0)));
			m_vertices.push_back(Vertex(Location(initialPos.x + size, initialPos.y + size, 0)));
			m_verticesLink = {	0, 1, 2,
								1, 2, 3
			};

			m_attributesPositions.push_back(3);
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

std::vector<unsigned int> Geometry::getVerticesLink() const {
	return m_verticesLink;
}

std::vector<unsigned int> Geometry::getAttributesPositions() const {
	return m_attributesPositions;
}
