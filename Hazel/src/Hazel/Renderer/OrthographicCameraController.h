#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"


namespace Hazel
{

	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep timestep);
		void OnEvent(Event& e);
		OrthographicCamera& GetCamera() { return m_Camera; };
		const OrthographicCamera& GetCamera() const { return m_Camera; };

		void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
		float GetZoomLevel() const { return m_ZoomLevel; }

		const OrthographicCameraBounds& GetBounds() const { return m_Bounds; }

	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnMouseMoved(MouseMovedEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCameraBounds m_Bounds;
		OrthographicCamera m_Camera;

		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f; // In degrees, in anti-clockwise direction
		float m_CameraTranslationSpeed = 1.0f;
		float m_CameraRotationSpeed = 1.0f;
		float m_ShiftSpeedMultiplier = 20.f;

	};

}
