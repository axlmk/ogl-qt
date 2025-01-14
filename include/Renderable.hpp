#pragma once
#include <filesystem>
#include "Shader.hpp"
#include "Geometry.hpp"

class Renderable {
public:

	// TODO : rule of 3
	Renderable();
	Renderable(std::shared_ptr<Geometry> geometry, std::shared_ptr<Shader> shader);
	~Renderable();

	std::shared_ptr<Geometry> getGeometry() const;
	std::shared_ptr<Shader> getShader() const;

	void linkGeo(std::shared_ptr<Geometry> geometry);
	void linkShader(std::shared_ptr<Shader> shader);
	void render() const;

private:

	unsigned int m_vbo;
	unsigned int m_vao;
	unsigned int m_ebo;

	std::string m_err_msg;

	std::shared_ptr<Geometry> m_geo;
	std::shared_ptr<Shader> m_shd;

	void generateRender();
	void deleteBuffers();
};
