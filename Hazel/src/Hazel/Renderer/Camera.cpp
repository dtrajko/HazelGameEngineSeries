#include "hzpch.h"

#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"


namespace Hazel
{
	Camera::Camera()
	{
	}

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
		m_ProjectionMatrix = glm::perspective(m_FOV, m_Aspect, m_Near, m_Far);
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::Update(Timestep ts)
	{
	}

	float Camera::GetExposure()
	{
		return 0.0f;
	}

}
