#pragma once

#include "Hazel/Renderer/CameraDT.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"


namespace Hazel
{

	class CameraController
	{
	public:
		CameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		CameraDT& GetCamera() { return m_Camera; };
		const CameraDT& GetCamera() const { return m_Camera; };

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		float GetZoomLevel() const { return m_ZoomLevel; }

		void SetTranslationSpeed(float cameraTranslationSpeed) { m_CameraTranslationSpeed = cameraTranslationSpeed; };
		void SetRotationSpeed(float cameraRotationSpeed) { m_CameraRotationSpeed = cameraRotationSpeed; };

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		CameraDT m_Camera;

		bool m_Rotation;

		float m_CursorOffsetX = 0.0f; // Yaw
		float m_CursorOffsetY = 0.0f; // Pitch
		float m_LastMousePositionX = 0.0f;
		float m_LastMousePositionY = 0.0f;
		bool m_MouseFirstMoved = true;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 4.0f };
		glm::vec3 m_CameraRotation = { 0.0f, 180.0f, 0.0f };
		float m_CameraTranslationSpeed = 5.0f;
		float m_CameraRotationSpeed = 1.0f;
		float m_ShiftSpeedMultiplier = 10.f;
	};
}
