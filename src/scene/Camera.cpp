//! @file Camera.cpp
/*!
 * @date	05.01.2013
 * @author	Guido Schmidt
 */

#include "Camera.h"

namespace scene {

	Camera::Camera(glm::ivec2 window_size)
	{
		m_initPosition = glm::vec3(0.0, 0.0, 1.0);
		m_lookAt	   = glm::vec3(0.0, 0.0, 0.0);
		m_up		   = glm::vec3(0.0, 1.0, 0.0);
		m_side		   = glm::vec3(1.0, 0.0, 0.0);

		SetSide(m_side);
		SetUp(m_up);
		SetLookAt(m_lookAt);
		SetPosition(m_initPosition);

		m_fieldOfView = 60.0f;
		m_width 	  = window_size.x;
		m_height 	  = window_size.y;
		m_aspect 	  = static_cast<float>(m_width) / static_cast<float>(m_height);
		m_nearPlane   = 0.1f;
		m_farPlane 	  = 200.0f;

		m_viewMatrix	   	= glm::lookAt(m_initPosition, m_lookAt, m_up);
		m_inverseViewMatrix = glm::inverse(m_viewMatrix);
		m_projectionMatrix 	= glm::perspective(m_fieldOfView, m_aspect, m_nearPlane, m_farPlane);
		m_isOrtho 		   	= false;

		m_speed = 0.05f;
	}

	Camera::Camera(std::string name, glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, glm::ivec2 window_size)
	{
		m_name = name;

		m_initPosition = position;
		m_initPosition = position;

		m_initLookAt = lookAt;
		m_lookAt = lookAt;

		m_initUp = up;
		m_up = up;

		m_lookVec 	= m_initLookAt - m_initPosition;
		m_initSide 	= glm::cross(m_lookVec, m_initUp);
		m_side 		= glm::cross(m_lookVec, m_initUp);

		SetSide(m_side);
		SetUp(m_up);
		SetLookAt(m_lookAt);
		SetPosition(m_initPosition);

		m_fieldOfView = 60.0f;
		m_width 	  = window_size.x;
		m_height 	  = window_size.y;
		m_aspect 	  = static_cast<float>(m_width) / static_cast<float>(m_height);
		m_nearPlane   = 0.1f;
		m_farPlane 	  = 200.f;

		m_viewMatrix	   	= glm::lookAt(m_initPosition, m_lookAt, m_up);
		m_inverseViewMatrix = glm::inverse(m_viewMatrix);
		m_projectionMatrix  = glm::perspective(m_fieldOfView, m_aspect, m_nearPlane, m_farPlane);
		m_isOrtho 		    = false;

		m_speed = 0.05f;
	}

	Camera::~Camera() {
		// TODO Auto-generated destructor stub
	}

	glm::vec3 Camera::GetSide(void)
	{
		return glm::vec3(m_inverseViewMatrix[0]);
	}

	glm::vec3 Camera::GetUp(void)
	{
		return glm::vec3(m_inverseViewMatrix[1]);
	}

	glm::vec3 Camera::GetLookAt(void)
	{
		return glm::vec3(m_inverseViewMatrix[2]);
	}

	glm::vec3 Camera::GetPosition(void)
	{
		return glm::vec3(m_inverseViewMatrix[3]);
	}

	float Camera::GetFOV(void)
	{
		return m_fieldOfView;
	}

	float Camera::GetAspectRatio(void)
	{
		return m_aspect;
	}

	int Camera::GetWidth(void)
	{
		return m_width;
	}

	int Camera::GetHeight(void)
	{
		return m_height;
	}

	float Camera::GetNearPlane(void)
	{
		return m_nearPlane;
	}

	float Camera::GetFarPlane(void)
	{
		return m_farPlane;
	}

	glm::mat4 Camera::GetViewMatrix(void)
	{
		return glm::inverse(m_inverseViewMatrix);
	}

	glm::mat4 Camera::GetProjectionMatrix(void)
	{
		return m_projectionMatrix;
	}

	void Camera::SetSide(const glm::vec3& side)
	{
		m_inverseViewMatrix[0] = glm::vec4(side, 1.0);
	}

	void Camera::SetUp(const glm::vec3 up)
	{
		m_inverseViewMatrix[1] = glm::vec4(up, 1.0);
	}

	void Camera::SetLookAt(const glm::vec3 lookat)
	{
		m_inverseViewMatrix[2] = glm::vec4(lookat, 1.0);
	}

	void Camera::SetPosition(const glm::vec3 pos)
	{
		m_inverseViewMatrix[3] = glm::vec4(pos, 1.0);
	}

	void Camera::SetOrientation(const glm::vec3 side, const glm::vec3 up, const glm::vec3 lookat)
	{
			m_inverseViewMatrix[0] = glm::vec4(side, 0.0f);
			m_inverseViewMatrix[1] = glm::vec4(up, 0.0f);
			m_inverseViewMatrix[2] = glm::vec4(lookat, 0.0f);

			SetPosition(GetPosition());
	}

	void Camera::SetFOV(float fov)
	{
		m_fieldOfView = fov;
		if(m_isOrtho)
			m_projectionMatrix = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, m_nearPlane, m_farPlane);
		else
			m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspect, m_nearPlane, m_farPlane);
	}

	void Camera::SetAspectRatio(float aspect)
	{
		m_aspect = aspect;
	}

	void Camera::SetNearPlane(float near)
	{
		m_nearPlane = near;
	}

	void Camera::SetFarPlane(float far)
	{
		m_farPlane = far;
	}

	void Camera::SetProjection(float field_of_view, float aspect, float near, float far)
	{
	  m_fieldOfView = field_of_view;
	  m_aspect	= aspect;
	  m_nearPlane	= near;
	  m_farPlane	= far;

	  m_projectionMatrix = glm::perspective(m_fieldOfView, m_aspect, m_nearPlane, m_farPlane);
	}

	void Camera::MoveZ(float speed)
	{
		SetPosition(GetPosition() - GetLookAt() * speed);
	}

	void Camera::MoveX(float speed)
	{
		SetPosition(GetPosition() + GetSide() * speed);
	}

	void Camera::MoveY(float speed)
	{
		SetPosition(GetPosition() + GetUp() * speed);
	}

	void Camera::Rotate(float x, float y)
	{
		RotateByQuat(x, glm::vec3(0.0f, 1.0f, 0.0f));
		RotateByQuat(y, GetSide());
	}

	void Camera::RotateByQuat(const float angle, const glm::vec3 axis)
	{
		glm::vec3 view = GetLookAt();
		glm::vec3 up(0.0f, 1.0f, 0.0f);
		glm::quat quatView(0, view);
		glm::quat quatViewResult = glm::rotate(quatView, angle, axis);
		view.x = quatViewResult.x;
		view.y = quatViewResult.y;
		view.z = quatViewResult.z;
		view    = glm::normalize(view);
		glm::vec3 side = glm::cross(up, view);
		side = glm::normalize(side);
		up = glm::cross(view, side);
		up = glm::normalize(up);
		SetOrientation(side, up, view);
	}
}
