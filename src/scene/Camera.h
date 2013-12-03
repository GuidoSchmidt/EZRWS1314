//! @file Camera.h
/*!
 *	@date 	30.09.2013
 *	@author	Guido Schmidt
 */
#pragma once
#ifndef H_CAMERA
#define H_CAMERA

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/core/type_gentype.hpp>
#include <glm/gtx/quaternion.hpp>

namespace scene {
	//! @class Camera
	class Camera {

		private:
			//! Initial Values
			glm::vec3 m_initPosition;
			glm::vec3 m_initLookAt;
			glm::vec3 m_initUp;
			glm::vec3 m_initSide;

			//! View
			glm::vec3 m_lookVec;
			glm::vec3 m_lookAt;
			glm::vec3 m_up;
			glm::vec3 m_side;

			//! Perspective
			float m_fieldOfView;
			float m_aspect;
			int	  m_width, m_height;
			float m_farPlane, m_nearPlane;
			bool  m_isOrtho;

			//! Matrices
			glm::mat4 m_viewMatrix, m_inverseViewMatrix, m_projectionMatrix;

			//! Movement
			float m_speed;

		protected:
			//! \brief Checks if camera has changed
			void Changed(void);

			//! \brief Rotates the camera using a quaternion
			void RotateByQuat(const float angle, const glm::vec3 axis);

		public:
			//! \brief Constructor
			Camera(glm::ivec2 window_size);

			//! \brief Constructor
			Camera(glm::vec3 posVec, glm::vec3 lookAtVec, glm::vec3 upVec, glm::ivec2 window_size);
			
			//! \brief Destructor
			virtual ~Camera();

			//! \brief Returns side vector
			glm::vec3 GetSide(void);

			//! \brief Returns up vector
			glm::vec3 GetUp(void);

			//! \brief Returns look at position
			glm::vec3 GetLookAt(void);

			//! \brief Returns position
			glm::vec3 GetPosition(void);

			//! \brief Returns field of view
			float GetFOV(void);

			//! \brief Returns aspect ratio
			float GetAspectRatio(void);

			//! \brief Returns width of image plane
			int GetWidth(void);

			//! \brief Returns height of image plane
			int GetHeight(void);

			//! \brief Returns near plane value
			float GetNearPlane(void);

			//! \brief Returns far plane value
			float GetFarPlane(void);

			//! \brief Returns the view matrix
			glm::mat4 GetViewMatrix(void);

			//! \brief Returns the projection matrix
			glm::mat4 GetProjectionMatrix(void);

			//! \brief Sets the side vector
			void SetSide(const glm::vec3& side);

			//! \brief Sets the up vector
			void SetUp(const glm::vec3 up);

			//! \brief Sets the look at position
			void SetLookAt(const glm::vec3 lookat);
			
			//! \brief Sets the camera position
			void SetPosition(const glm::vec3 pos);

			//! \brief Sets the whole rotation: side, up, look at
			void SetOrientation(const glm::vec3 side, const glm::vec3 up, const glm::vec3 lookat);
			
			//! \brief Sets the field of view
			void SetFOV(float fov);

			//! \brief Sets the aspect ratio
			void SetAspectRatio(float aspect);

			//! \brief Sets the near plane value
			void SetNearPlane(float near);

			//! \brief Sets the far plane value
			void SetFarPlane(float far);

			//! \brief Moves the camera in x axis
			void MoveX(float speed);

			//! \brief Moves the camera in y axis
			void MoveY(float speed);

			//! \brief Moves the camera in z axis
			void MoveZ(float speed);

			//! \brief Rotates the camera by angle x around x-axis and angle y around y-axis
			void Rotate(float x, float y);
	};
}
#endif //H_CAMERA
