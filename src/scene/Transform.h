//! @file Transform.h
#pragma once
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace ezr {
	namespace scene {
		//! @class Transform
		class Transform
		{
			public:
				Transform( glm::vec3 position = glm::vec3(0.0), glm::quat rotation = glm::quat(1, glm::vec3(0.0)), glm::vec3 scale = glm::vec3(1.0) );
				~Transform(void);

				//! Getter
				glm::vec3 getTranslation(void);
				glm::quat getRotation(void);
				glm::vec3 getScale(void);
				glm::mat4 getModelMatrix(void);
				//! Setter
				void setPosition(const glm::vec3 position);
				void setRotation(const glm::quat rotate);
				void setScale(const glm::vec3 scale);

				static glm::quat axelAngusToQuat (const double angle, const glm::vec3 axis);
				static double	 angleFromQuat (const glm::quat q);
				static glm::vec3 axisFromQuat (const glm::quat q);
				static glm::quat quatFromAngular (const glm::vec3 omega);

			private:
				glm::vec3 m_translation;
				glm::quat m_rotation;
				glm::vec3 m_scale;

				glm::mat4 m_translation_matrix;
				glm::mat4 m_rotation_matrix;
				glm::mat4 m_scale_matrix;
				glm::mat4 m_model_matrix;
		};
	}
}
#endif //TRANSFORM_H
