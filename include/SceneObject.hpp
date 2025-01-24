#pragma once
#include <filesystem>
#include "Shader.hpp"
#include "Geometry.hpp"
#include "Camera.hpp"

class SceneObject {
public:

	// TODO : rule of 3
	SceneObject();
	SceneObject(std::shared_ptr<Geometry> geometry, std::shared_ptr<Shader> shader);
	~SceneObject();

	std::shared_ptr<Geometry> getGeometry() const;
	std::shared_ptr<Shader> getShader() const;

	void linkGeo(std::shared_ptr<Geometry> geometry);
	void linkShader(std::shared_ptr<Shader> shader);
	void generateRender();
	void render(std::shared_ptr<Camera> camera) const;

private:

	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ebo;

	std::string m_err_msg;

	std::shared_ptr<Geometry> m_geo;
	std::shared_ptr<Shader> m_shd;

	void deleteBuffers();
};
