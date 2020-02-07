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
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

}
