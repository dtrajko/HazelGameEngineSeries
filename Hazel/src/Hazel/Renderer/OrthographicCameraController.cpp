#include "hzpch.h"

#include "OrthographicCameraController.h"
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseButtonCodes.h"


namespace Hazel
{

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Rotation(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_CameraPosition(0.0f)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep timestep)
	{
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
			if (Input::IsKeyPressed(HZ_KEY_Q))
			{
				m_CameraRotation += m_CameraRotationSpeed * timestep.GetSeconds();
			}
			else if (Input::IsKeyPressed(HZ_KEY_E))
			{
				m_CameraRotation -= m_CameraRotationSpeed * timestep.GetSeconds();
			}

			m_Camera.SetRotation(m_CameraRotation);
		}
		/* Camera movement End */

		m_Camera.SetPosition(m_CameraPosition);

		m_CameraTranslationSpeed = m_ZoomLevel;

	}

	void OrthographicCameraController::OnEvent(Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseMoved));
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResize));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.1f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1))
		{
			HZ_INFO("Mouse position X = {0}, Y = {1}", e.GetX(), e.GetY());
		}

		return false;
	}

	bool OrthographicCameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
