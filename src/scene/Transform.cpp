//! @file Transform.cpp
#include "Transform.h"

namespace scene {
    Transform::Transform(glm::vec3 position, glm::quat rotate, glm::vec3 scale)
	{
        m_position    = position;
		m_rotation	  = rotate;
		m_scale		  = scale;

		m_translation_matrix = glm::mat4(0.0);
        m_translation_matrix = glm::translate(m_position);
		m_rotation_matrix	 = glm::mat4(0.0);
		m_rotation_matrix	 = glm::toMat4(m_rotation);
		m_scale_matrix		 = glm::mat4(0.0);
		m_scale_matrix		 = glm::scale(m_scale);

		m_model_matrix = glm::mat4(0.0);
		m_model_matrix = m_scale_matrix * m_rotation_matrix * m_translation_matrix;
	}

	Transform::~Transform(void)
	{
	}

	glm::mat4 Transform::getModelMatrix(void)
	{
		m_model_matrix = m_translation_matrix * m_rotation_matrix * m_scale_matrix;
		return m_model_matrix;
	}

	glm::vec3 Transform::getPosition(void)
	{
        return m_position;
	}

	glm::quat Transform::getRotation(void)
	{
		return m_rotation;
	}

	glm::vec3 Transform::getScale(void)
	{
		return m_scale;
	}

	void Transform::setPosition(glm::vec3 position)
	{
        m_position = position;
        m_translation_matrix = glm::translate(m_position);
	}

	void Transform::setRotation(glm::quat rotate)
	{
		m_rotation = rotate;
		m_rotation_matrix	 = glm::toMat4(m_rotation);
	}

	void Transform::setScale(glm::vec3 scale)
	{
		m_scale = scale;
		m_scale_matrix = glm::scale(m_scale);
	}

    void Transform::translate(float x, float y, float z)
    {
        m_position.x += x;
        m_position.y += y;
        m_position.z += z;
    }

	glm::quat Transform::axelAngusToQuat (const double angle, const glm::vec3 axis)
	{
		return glm::quat(cos(angle*0.5), axis.x * sin(angle*0.5),axis.y * sin(angle*0.5),axis.z * sin(angle*0.5));
	}

	double Transform::angleFromQuat (const glm::quat q)
	{
		return 2*acos(q.w);
	}

	glm::quat Transform::quatFromAngular(glm::vec3 omega)
	{
		double angle = glm::length(omega);
		glm::vec3 norm = omega / glm::length(omega);
		return glm::quat(cos(angle/2), norm);
	}
}
