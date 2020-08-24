#include "hzpch.h"

#include "OrthographicCameraController.h"
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"


namespace Hazel
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom,  m_Bounds.Top), m_Rotation(rotation)
	{
		HZ_PROFILE_FUNCTION();
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
		HZ_PROFILE_FUNCTION();

		/* Camera movement Begin */
		if (Input::IsKeyPressed(HZ_KEY_LEFT) || Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition.x -= m_CameraTranslationSpeed * timestep.GetSeconds();
		}
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT) || Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition.x += m_CameraTranslationSpeed * timestep.GetSeconds();
		}

		if (Input::IsKeyPressed(HZ_KEY_UP) || Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition.y += m_CameraTranslationSpeed * timestep.GetSeconds();
		}
		else if (Input::IsKeyPressed(HZ_KEY_DOWN) || Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition.y -= m_CameraTranslationSpeed * timestep.GetSeconds();
		}

		if (m_Rotation)
		{
			float shiftSpeedMultiplier = Input::IsKeyPressed(HZ_KEY_LEFT_SHIFT) ? m_ShiftSpeedMultiplier : 1.f;

			if (Input::IsKeyPressed(HZ_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * shiftSpeedMultiplier * timestep.GetSeconds();
			}
			else if (Input::IsKeyPressed(HZ_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * shiftSpeedMultiplier * timestep.GetSeconds();
			}

			m_Camera.SetRotation(m_CameraRotation);
		}
		/* Camera movement End */

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		HZ_PROFILE_FUNCTION();

		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseMoved));
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	void OrthographicCameraController::OnResize(float width, float height)
	{
		HZ_PROFILE_FUNCTION();

		m_AspectRatio = width / height;
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
	}

	void OrthographicCameraController::SetZoomLevel(float zoomLevel)
	{
		HZ_PROFILE_FUNCTION();

		m_ZoomLevel = zoomLevel;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		CalculateView();
	}

	void OrthographicCameraController::CalculateView()
	{
		HZ_PROFILE_FUNCTION();

		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		CalculateView();
		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1))
		{
			// HZ_INFO("Mouse position X = {0}, Y = {1}", e.GetX(), e.GetY());
		}

		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		HZ_PROFILE_FUNCTION();

		OnResize((float)e.GetWidth(), (float)e.GetHeight());
		return false;
	}
}
