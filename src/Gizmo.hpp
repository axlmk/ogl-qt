#pragma once
#include "Model.hpp"
#include "SceneObject.hpp"
#include "glm/glm.hpp"

class Gizmo : public SceneObject
{
   public:
	enum ArrowDirection { X, Y, Z, None };

	explicit Gizmo();

	/**
	 * @brief Load the gizmo's model and compile it's shader
	 */
	void load(void);

	void render(const Camera& camera, const std::vector<LightProperties*>& lights) const override;

	void renderPicking(const Camera& camera) const override;

	/**
	 * @brief Indicates if the \p id matches the Gizmo's id
	 * @param[in] id The id of the object to compare
	 * @return The indication
	 */
	bool isId(const glm::ivec3& id) const override;

	/**
	 * @brief Select the gizmo
	 * @param[in] id The id of the arrow to select
	 */
	void select(glm::ivec3 id);

	/**
	 * @brief Unselect the gizmo
	 */
	void unselect(void);

	/**
	 * @brief Indicates if the gizmo is selected or not
	 * @return The selection state
	 */
	bool isSelected(void) const;

	int getSelectedIndex(void) const;

	/**
	 * @brief Get the color id of the given arrow
	 * @param[in] arrow The arrow
	 * @return The color id
	 */
	glm::vec3 _getColorId(ArrowDirection arrow) const;

   private:
	/**
 	 * @brief Model one arrow of the gizmo
     */
	struct Arrow
	{
		Model mdl;			 ///< The model
		Shader shd;			 ///< The Shader
		glm::ivec3 colorId;	 ///< The color ID
	};

	ArrowDirection m_selectedArrow;	 ///< Represents the selected arrow of the gizmo
	std::array<Arrow, 3> m_arrows;	 ///< The three arrows composing the gizmo
	glm::vec3 m_position;			 ///< The position of the gizmo

	constexpr static float m_scalingFactor = 0.5f;	///< Scaling factor of the gizmo
	constexpr static float m_threshold = 0.25;		///< The threshold from which an arrow is seen or not
};