#include "hzpch.h"

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Hazel
{

	Camera::Camera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(1.0f), m_ViewMatrix(1.0f)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::RecalculateViewMatrix()
	{
		float Pitch = m_Rotation.x;
		float Yaw = m_Rotation.y;

		m_Front.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw)); // cos(pitch) * sin(yaw)
		m_Front.y = sin(glm::radians(Pitch));                          // sin(pitch)
		m_Front.z = cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)); // cos(pitch) * cos(yaw)
		m_Front = glm::normalize(m_Front);

		m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		// HZ_TRACE("Camera UP vector X: {0}, Y: {1}, Z: {2}", m_Up.x, m_Up.y, m_Up.z);

		m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
