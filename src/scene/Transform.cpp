//! @file Transform.cpp
#include "Transform.h"

namespace ezr {
	namespace scene {
		//! \brief Constructor
		Transform::Transform(glm::vec3 translate, glm::quat rotate, glm::vec3 scale)
		{
			m_translation = translate;
			m_rotation	  = rotate;
			m_scale		  = scale;

			m_translation_matrix = glm::mat4(0.0);
			m_translation_matrix = glm::translate(m_translation);
			m_rotation_matrix	 = glm::mat4(0.0);
			m_rotation_matrix	 = glm::toMat4(m_rotation);
			m_scale_matrix		 = glm::mat4(0.0);
			m_scale_matrix		 = glm::scale(m_scale);

			m_model_matrix = glm::mat4(0.0);
			m_model_matrix = m_scale_matrix * m_rotation_matrix * m_translation_matrix;
		}

		//! \brief Destructor
		Transform::~Transform(void)
		{
		}

		//! \brief Returns the accumulated model matrix
		glm::mat4 Transform::getModelMatrix(void)
		{
			m_model_matrix = m_translation_matrix * m_rotation_matrix * m_scale_matrix;
			return m_model_matrix;
		}

		//! \brief Returns the translation vector (vec3)
		glm::vec3 Transform::getTranslation(void)
		{
			return m_translation;
		}

		//! \brief Returns the rotation vector (quaternion)
		glm::quat Transform::getRotation(void)
		{
			return m_rotation;
		}

		//! \brief Returns the scale vector (vec3)
		glm::vec3 Transform::getScale(void)
		{
			return m_scale;
		}

		//! \brief Sets the translation vector (vec3)
		void Transform::setPosition(glm::vec3 position)
		{
			m_translation = position;
			m_translation_matrix = glm::translate(m_translation);
		}

		//! \brief Sets the rotation vector (vec4: angle + xyz)
		void Transform::setRotation(glm::quat rotate)
		{
			m_rotation = rotate;
			m_rotation_matrix	 = glm::toMat4(m_rotation);
		}

		//! \brief Sets the scale vector (vec3)
		void Transform::setScale(glm::vec3 scale)
		{
			m_scale = scale;
			m_scale_matrix = glm::scale(m_scale);
		}

		glm::quat Transform::axelAngusToQuat (const double angle, const glm::vec3 axis)
		{
			return glm::quat(cos(angle*0.5), axis.x * sin(angle*0.5),axis.y * sin(angle*0.5),axis.z * sin(angle*0.5));
		}
		double Transform::angleFromQuat (const glm::quat q)
		{
			return 2*acos(q.w);
		}
		// TODO: not sure if angle is correct. ... have to check with computation of w.
		glm::quat Transform::quatFromAngular(glm::vec3 omega)
		{
			double angle = glm::length(omega);
			glm::vec3 norm = omega / glm::length(omega);
			return glm::quat(cos(angle/2), norm);
		}
	}
}
