#pragma once
#include <filesystem>
#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

class SceneObject {
public:

	// TODO : rule of 3
	SceneObject();
	SceneObject(Geometry* geometry, Shader* shader);
	~SceneObject();

	Geometry* getGeometry() const;
	Shader* getShader() const;

	void linkGeo(Geometry* geometry);
	void linkShader(Shader* shader);
	void generateRender();
	void render(Camera* camera) const;

private:

	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ebo;

	std::string err_msg;

	Geometry* m_geo;
	Shader* m_shd;

	void deleteBuffers();
};
