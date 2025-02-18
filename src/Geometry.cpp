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
	m_attributesPositions{ VERTEX_SPACE_COORD_SIZE, VERTEX_NORMAL_SIZE, VERTEX_TEXT_COORD_SIZE },
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
			m_vertices.push_back(Vertex({ 0,	0,		0 }));
			m_vertices.push_back(Vertex({ size, 0,		0 }));
			m_vertices.push_back(Vertex({ 0,	size,	0 }));
			m_vertices.push_back(Vertex({ size, size,	0 }));
			m_vertices.push_back(Vertex({ 0,	0,		size }));
			m_vertices.push_back(Vertex({ size, 0,		size }));
			m_vertices.push_back(Vertex({ 0,	size,	size }));
			m_vertices.push_back(Vertex({ size, size,	size }));

			//m_verticesLink = {	0, 1, 2, // 0
			//					1, 2, 3,
			//					2, 3, 6, // 1
			//					6, 7, 3,
			//					3, 1, 5, // 2
			//					3, 5, 7,
			//					2, 6, 4, // 3
			//					2, 0, 4,
			//					6, 7, 5, // 4
			//					4, 5, 6,
			//					0, 1, 4,
			//					1, 4, 5

			//};

			m_verticesLink = {
				2, 5, 8,
				5, 8, 11,
				7, 10, 19,
				19, 22, 10,
				9, 3, 15,
				9, 15, 21,
				6, 18, 12,
				6, 0, 12,
				20, 23, 17,
				14, 17, 20,
				1, 4, 13,
				4, 13, 16
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

	// hardcoded, for a cube its 3
	int facesNumber = 3;
	floatVertices.reserve(m_vertices.size() * getStrideLength() * facesNumber);

	for (size_t i = 0; i < m_vertices.size() ; i++)
	{
		std::vector<TextCoord> tmpText;
		std::vector<SpaceCoord> tmpNormals;
		if(m_hasTexture)
			tmpText = m_vertices[i].getTextCoord();
		if(m_hasNormals)
			tmpNormals = m_vertices[i].getNormals();

		for(size_t j = 0; j < facesNumber; j++)
		{
			const std::vector<float>& tmpFloat = m_vertices[i].getSpaceCoord();
			std::copy(tmpFloat.begin(), tmpFloat.end(), std::back_inserter(floatVertices));

			if (!tmpText.empty())
			{
				floatVertices.push_back(tmpText[j].x);
				floatVertices.push_back(tmpText[j].y);
			}

			if(m_hasNormals)
			{
				floatVertices.push_back(tmpNormals[j].x);
				floatVertices.push_back(tmpNormals[j].y);
				floatVertices.push_back(tmpNormals[j].z);
			}
		}
	}
	return floatVertices;
}


std::vector<unsigned int> Geometry::getVerticesLink() const {
	return m_verticesLink;
}



std::vector<unsigned int> Geometry::getAttributesPositions() const {
	int n = 1;
	if(m_hasNormals)
		n++;
	if(m_hasTexture)
		n++;
	return std::vector<unsigned int> (m_attributesPositions.begin(), m_attributesPositions.begin() + n);
}



unsigned int Geometry::getStrideLength() const {
	int len = VERTEX_SPACE_COORD_SIZE;
	if(m_hasTexture)
		len += VERTEX_TEXT_COORD_SIZE;
	if(m_hasNormals)
		len += VERTEX_NORMAL_SIZE;
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
	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), m_rotation.x, { m_rotation.y, m_rotation.z, m_rotation.w });
	glm::vec4 rotated = rotation * glm::vec4(m_position.x, m_position.y, m_position.z, 1.0);
	return { rotated.x, rotated.y, rotated.z };
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

void Geometry::setNormals() {
	switch (m_geoType) {
		case GeometryType::Cube:
			m_vertices[0].setNormals({ { -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 0.0, -1.0 } });
			m_vertices[1].setNormals({ {  1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 0.0, -1.0 } });
			m_vertices[2].setNormals({ { -1.0, 0.0, 0.0 }, { 0.0,  1.0, 0.0 }, { 0.0, 0.0, -1.0 } });
			m_vertices[3].setNormals({ {  1.0, 0.0, 0.0 }, { 0.0,  1.0, 0.0 }, { 0.0, 0.0, -1.0 } });
			m_vertices[4].setNormals({ { -1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 0.0,  1.0 } });
			m_vertices[5].setNormals({ {  1.0, 0.0, 0.0 }, { 0.0, -1.0, 0.0 }, { 0.0, 0.0,  1.0 } });
			m_vertices[6].setNormals({ { -1.0, 0.0, 0.0 }, { 0.0,  1.0, 0.0 }, { 0.0, 0.0,  1.0 } });
			m_vertices[7].setNormals({ {  1.0, 0.0, 0.0 }, { 0.0,  1.0, 0.0 }, { 0.0, 0.0,  1.0 } });
			break;
		case GeometryType::Square:
		case GeometryType::Model:
		default:
			break;
	}
	m_hasNormals = true;
}

void Geometry::unsetTextureMapping() {
	m_hasTexture = false;
}