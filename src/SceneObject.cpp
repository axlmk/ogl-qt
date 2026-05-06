#include "SceneObject.hpp"

#include <functional>

#include "Camera.hpp"
#include "LightProperties.hpp"
#include "Model.hpp"

Shader SceneObject::pick{ShaderType::Unicolor};

SceneObject::SceneObject() : m_model{nullptr}, m_shd{nullptr} {}

SceneObject::SceneObject(Model* geometry, Shader* shader)
	: m_transformation{glm::vec3(0.f), glm::vec3(1.f), glm::vec3(0.f)}, m_model{geometry}, m_shd{shader}, m_loaded{false}
{}

Model* SceneObject::getModel() const
{
	return m_model;
}

void SceneObject::load(void)
{
	m_model->load();
	m_shd->compile();
}

void SceneObject::_setUpLights(const Camera& camera, const std::vector<LightProperties*>& lights) const
{
	int uniform = m_shd->getUniform("nLights");
	g_opengl.glUniform1i(uniform, (int)lights.size());

	int i = 0;
	for (const auto& light : lights)
	{
		std::string iStr = std::to_string(i++);

		uniform = m_shd->getUniform("lights[" + iStr + "].type");
		g_opengl.glUniform1i(uniform, light->type);

		uniform = m_shd->getUniform("lights[" + iStr + "].intensity");
		g_opengl.glUniform1f(uniform, light->intensity);

		switch (light->type)
		{
			case LightProperties::LightType::Point: {

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light->position.x, light->position.y, light->position.z);

				uniform = m_shd->getUniform("lights[" + iStr + "].linear");
				g_opengl.glUniform1f(uniform, light->linear);

				uniform = m_shd->getUniform("lights[" + iStr + "].quadratic");
				g_opengl.glUniform1f(uniform, light->quadratic);
				break;
			}
			case LightProperties::LightType::Spot: {
				// The light has inverted outerCutoff and cutoff and I'm too lazy to investigate why, so I just swap them here
				uniform = m_shd->getUniform("lights[" + iStr + "].cutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(light->outerCutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].outerCutoff");
				g_opengl.glUniform1f(uniform, glm::cos(glm::radians(light->cutoff)));

				uniform = m_shd->getUniform("lights[" + iStr + "].position");
				g_opengl.glUniform3f(uniform, light->position.x, light->position.y, light->position.z);
				[[fallthrough]];
			}
			case LightProperties::LightType::Directional: {
				uniform = m_shd->getUniform("lights[" + iStr + "].direction");
				g_opengl.glUniform3f(uniform, light->direction.x, light->direction.y, light->direction.z);
				break;
			}
			default: {
				break;
			}
		}

		uniform = m_shd->getUniform("lights[" + iStr + "].diffuse");
		g_opengl.glUniform3f(uniform, 0.65f, 0.65f, 0.65f);

		uniform = m_shd->getUniform("lights[" + iStr + "].specular");
		g_opengl.glUniform3f(uniform, 1.0f, 1.0f, 1.0f);
	}

	glm::vec3 cameraPos = camera.getPosition();
	uniform = m_shd->getUniform("cameraPos");
	g_opengl.glUniform3f(uniform, cameraPos.x, cameraPos.y, cameraPos.z);
}

void SceneObject::render(const Camera& camera, const std::vector<LightProperties*>& lights) const
{
	_render(camera, lights, *m_model, *m_shd, m_transformation);
}

void SceneObject::renderSelected(const Camera& camera, const std::vector<LightProperties*>& lights, const Selection& selection) const
{
	_renderSelected(camera, lights, *m_model, *m_shd, selection);
}

void SceneObject::_render(const Camera& camera, const std::vector<LightProperties*>& lights, const Model& model, const Shader& shader,
						  const transformation& transformation) const
{
	g_opengl.glStencilMask(0x00);

	_setTransformation(camera, shader, transformation);

	// Lights calculations
	if (shader.getType() == ShaderType::Texture)
		_setUpLights(camera, lights);

	// Final render
	model.Draw(shader);
}

void SceneObject::_renderSelected(const Camera& camera, const std::vector<LightProperties*>& lights, const Model& model, const Shader& shader,
								  const Selection& selection) const
{
	g_opengl.glStencilMask(0xFF);
	g_opengl.glStencilFunc(GL_ALWAYS, 1, 0xFF);

	_setTransformation(camera, shader, m_transformation);

	// Lights calculations
	if (shader.getType() == ShaderType::Texture)
		_setUpLights(camera, lights);

	// Final render
	model.Draw(shader);

	// Selection rendering
	g_opengl.glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	g_opengl.glStencilMask(0x00);
	g_opengl.glDisable(GL_DEPTH_TEST);

	_setTransformation(camera, selection.color, {m_transformation.position, glm::vec3(selection.scale), m_transformation.rotation});

	model.Draw(selection.color);

	g_opengl.glStencilMask(0xFF);
	g_opengl.glStencilFunc(GL_ALWAYS, 1, 0xFF);
	g_opengl.glEnable(GL_DEPTH_TEST);
	g_opengl.glClear(GL_STENCIL_BUFFER_BIT);
}

void SceneObject::_setTransformation(const Camera& camera, const Shader& shader, const transformation& transformation) const
{
	glm::mat4 model_mat = glm::mat4(1.f);
	glm::mat4 view_mat = glm::mat4(1.f);
	glm::mat4 projection_mat = glm::mat4(1.f);

	shader.use();

	// World's location
	model_mat = glm::scale(glm::translate(glm::mat4(1.0f), transformation.position), transformation.scale);
	model_mat = glm::rotate(model_mat, glm::radians(transformation.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model_mat = glm::rotate(model_mat, glm::radians(transformation.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model_mat = glm::rotate(model_mat, glm::radians(transformation.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	// Camera's location
	view_mat = camera.getSpaceMat();

	// Camera's settings)
	projection_mat = glm::perspective(glm::radians(camera.getFov()), 600.f / 400.f, camera.getNearPlan(), camera.getFarPlan());

	// Combination of previous views
	shader.setTransformation(model_mat, view_mat, projection_mat);
}

void SceneObject::renderPicking(const Camera& camera) const
{
	_renderPicking(camera, *m_model, _getColorId(), m_transformation);
}

void SceneObject::setName(const std::string& name)
{
	m_name = name;
	std::hash<std::string> hasher;
	int32_t id = static_cast<int32_t>(hasher(m_name));

	m_colorId.x = id & 0xFF;
	m_colorId.y = (id >> 8) & 0xFF;
	m_colorId.z = (id >> 16) & 0xFF;
}

const std::string& SceneObject::getName(void) const
{
	return m_name;
}

void SceneObject::_renderPicking(const Camera& camera, const Model& model, const glm::vec3& colorId, const transformation& transformation) const
{
	pick.setColor(colorId);
	_setTransformation(camera, pick, transformation);

	// Final render
	model.Draw(pick);
}

glm::vec3 SceneObject::_getColorId(void) const
{
	return {m_colorId.x / 255.0, m_colorId.y / 255.0, m_colorId.z / 255.0};
}

bool SceneObject::isId(const glm::ivec3& id) const
{
	return id == m_colorId;
}

void SceneObject::scale(float scale)
{
	m_transformation.scale = glm::vec3(scale);
}

void SceneObject::scale(glm::vec3 scale)
{
	m_transformation.scale = scale;
}

void SceneObject::translate(const glm::vec3& translation)
{
	m_transformation.position += translation;
}

glm::vec3 SceneObject::getPosition(void) const
{
	return m_transformation.position;
}

glm::vec3 SceneObject::getRotation(void) const
{
	return m_transformation.rotation;
}

void SceneObject::setPosition(const glm::vec3& position)
{
	m_transformation.position = position;
}

void SceneObject::rotate(float angle, RotationAxis axis)
{
	switch (axis)
	{
		case X:
			m_transformation.rotation.x = angle;
			break;
		case Y:
			m_transformation.rotation.y = angle;
			break;
		case Z:
			m_transformation.rotation.z = angle;
			break;
	}
}
