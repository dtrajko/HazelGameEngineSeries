#pragma once

#include "Hazel/Core/Timestep.h"

#include <glm/glm.hpp>


namespace Hazel
{

	class Camera
	{
	public:

		Camera(float left, float right, float bottom, float top);
		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; };
		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); };
		glm::vec3 GetRotation() const { return m_Rotation; };
		inline void SetRotation(glm::vec3 rotation) { m_Rotation = rotation; RecalculateViewMatrix(); };
		inline void SetFOV(float FOV) { m_FOV = FOV; RecalculateViewMatrix(); };

		const glm::vec3& GetFront() const { return m_Front; };
		const glm::vec3& GetUp() const { return m_Up; };
		const glm::vec3& GetRight() const { return m_Right; };

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };

		void RecalculateViewMatrix();

		void Update(Timestep ts);

	public:
		glm::vec3 m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_Right = glm::vec3(1.0f, 0.0f, 0.0f);

	private:

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;

		glm::vec3 m_Position = { 0.0f, 0.0f, 4.0f };
		glm::vec3 m_Rotation = { 0.0f, 180.0f, 0.0f };

		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float m_FOV = glm::radians(45.0f);
		float m_Aspect = 1280.0f / 720.0f;
		float m_Near = 0.1f;
		float m_Far = 150.0f;

	};

}
