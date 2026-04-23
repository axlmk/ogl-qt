#include "gizmo.hpp"

#include "Camera.hpp"

Gizmo::Gizmo()
	: m_selectedArrow{ArrowDirection::None},
	  m_arrows{Arrow(Model(appDir.filePath("resources/models/gizmo/x.obj").toStdU16String()), Shader(ShaderType::Unicolor), {255, 0., 0.}),
			   Arrow(Model(appDir.filePath("resources/models/gizmo/y.obj").toStdU16String()), Shader(ShaderType::Unicolor), {0., 255, 0.}),
			   Arrow(Model(appDir.filePath("resources/models/gizmo/z.obj").toStdU16String()), Shader(ShaderType::Unicolor), {0., 0., 255})},
	  m_position{0.}
{}

void Gizmo::load(void)
{
	m_arrows[0].mdl.load();
	m_arrows[0].shd.setColor("#FF0000");
	m_arrows[0].shd.compile();

	m_arrows[1].mdl.load();
	m_arrows[1].shd.setColor("#00FF00");
	m_arrows[1].shd.compile();

	m_arrows[2].mdl.load();
	m_arrows[2].shd.setColor("#0000FF");
	m_arrows[2].shd.compile();
}

void Gizmo::render(const Camera& camera, [[maybe_unused]] const std::vector<LightProperties*>& lights) const
{
	const glm::vec3 A{0., 0., 1.};
	const glm::vec3 C{1., 0., 0.};
	const glm::vec3 D{0., 1., 0.};
	const auto B = camera.getDirection();

	const auto xCos = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
	const auto zCos = (glm::dot(C, B)) / (glm::length(C) * glm::length(B));
	const auto YCos = (glm::dot(D, B)) / (glm::length(D) * glm::length(B));

	const transformation transfo{m_transformation.position, glm::vec3(m_scalingFactor)};
	// Todo : need to decide if we scale the gizmo to make it have the same no matter the zoom or not
	// const auto lenCamArrow = glm::length(camera.getPosition() - m_position);

	if (std::abs(xCos) > m_threshold || std::abs(YCos) > m_threshold)
	{
		SceneObject::_render(camera, lights, m_arrows[0].mdl, m_arrows[0].shd, transfo);
	}

	SceneObject::_render(camera, lights, m_arrows[1].mdl, m_arrows[1].shd, transfo);

	if (std::abs(zCos) > m_threshold || std::abs(YCos) > m_threshold)
	{
		SceneObject::_render(camera, lights, m_arrows[2].mdl, m_arrows[2].shd, transfo);
	}
}

void Gizmo::renderPicking(const Camera& camera) const
{
	const glm::vec3 A{0., 0., 1.};
	const glm::vec3 C{1., 0., 0.};
	const glm::vec3 D{0., 1., 0.};
	const auto B = camera.getDirection();
	const auto xCos = (glm::dot(A, B)) / (glm::length(A) * glm::length(B));
	const auto zCos = (glm::dot(C, B)) / (glm::length(C) * glm::length(B));
	const auto YCos = (glm::dot(D, B)) / (glm::length(D) * glm::length(B));

	const transformation transfo{m_transformation.position, glm::vec3(m_scalingFactor)};

	// Todo : need to decide if we scale the gizmo to make it have the same no matter the zoom or not
	// const auto lenCamArrow = glm::length(camera.getPosition() - m_position);

	if (std::abs(xCos) > m_threshold || std::abs(YCos) > m_threshold)
	{
		_renderPicking(camera, m_arrows[0].mdl, _getColorId(ArrowDirection::X), transfo);
	}

	_renderPicking(camera, m_arrows[1].mdl, _getColorId(ArrowDirection::Y), transfo);

	if (std::abs(zCos) > m_threshold || std::abs(YCos) > m_threshold)
	{
		_renderPicking(camera, m_arrows[2].mdl, _getColorId(ArrowDirection::Z), transfo);
	}
}

glm::vec3 Gizmo::_getColorId(ArrowDirection arrow) const
{
	switch (arrow)
	{
		case ArrowDirection::X:
			return {1., 0., 0.};
		case ArrowDirection::Y:
			return {0., 1., 0.};
		case ArrowDirection::Z:
			return {0., 0., 1.};
		default:
			return {0., 0., 0.};
	}
}

bool Gizmo::isId(const glm::ivec3& id) const
{
	return id == glm::ivec3(255, 0, 0) || id == glm::ivec3(0, 255, 0) || id == glm::ivec3(0, 0, 255);
}

bool Gizmo::isSelected(void) const
{
	return m_selectedArrow != ArrowDirection::None;
}

int Gizmo::getSelectedIndex(void) const
{
	return m_selectedArrow;
}

void Gizmo::select(glm::ivec3 id)
{
	if ((id.x == 255) && (id.y == 0) && (id.z == 0))
	{
		m_selectedArrow = ArrowDirection::X;
	}
	else if ((id.x == 0) && (id.y == 255) && (id.z == 0))
	{
		m_selectedArrow = ArrowDirection::Y;
	}
	else if ((id.x == 0) && (id.y == 0) && (id.z == 255))
	{
		m_selectedArrow = ArrowDirection::Z;
	}
	else
	{
		m_selectedArrow = ArrowDirection::None;
	}
}

void Gizmo::unselect(void)
{
	select({0, 0, 0});
}