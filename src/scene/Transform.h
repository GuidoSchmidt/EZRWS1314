//! @file Transform.h
#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace scene {
	//! @class Transform
	class Transform
	{
		private:
			glm::vec3 m_translation;
			glm::quat m_rotation;
			glm::vec3 m_scale;

			glm::mat4 m_translation_matrix;
			glm::mat4 m_rotation_matrix;
			glm::mat4 m_scale_matrix;
			glm::mat4 m_model_matrix;

		public:
			//! \brief Constructor
			Transform( glm::vec3 position = glm::vec3(0.0), glm::quat rotation = glm::quat(1, glm::vec3(0.0)), glm::vec3 scale = glm::vec3(1.0) );
			
			//! \brief Destructor
			~Transform(void);

			//! \brief Returns the translation
			glm::vec3 getTranslation(void);

			//! \brief Returns the rotation
			glm::quat getRotation(void);

			//! \brief Returns the scale
			glm::vec3 getScale(void);

			//! \brief Returns the model matrix
			glm::mat4 getModelMatrix(void);
			
			//! \brief Sets the position
			void setPosition(const glm::vec3 position);

			//! \brief Sets the rotation
			void setRotation(const glm::quat rotate);

			//! \brief Sets the scale
			void setScale(const glm::vec3 scale);

			static glm::quat axelAngusToQuat (const double angle, const glm::vec3 axis);
			static double	 angleFromQuat (const glm::quat q);
			static glm::vec3 axisFromQuat (const glm::quat q);
			static glm::quat quatFromAngular (const glm::vec3 omega);
	};
}
#endif //TRANSFORM_H
