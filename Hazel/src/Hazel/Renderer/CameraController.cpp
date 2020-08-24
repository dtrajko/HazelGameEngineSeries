#include "hzpch.h"

#include "CameraController.h"
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"
#include "Hazel/Core/Application.h"


namespace Hazel
{

	CameraController::CameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio),
		m_Rotation(rotation),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{
	}

	void CameraController::OnUpdate(Timestep timestep)
	{
		if (Input::IsKeyPressed(HZ_KEY_UP) || Input::IsKeyPressed(HZ_KEY_W))
		{
			m_CameraPosition += m_Camera.GetFront() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}
		else if (Input::IsKeyPressed(HZ_KEY_DOWN) || Input::IsKeyPressed(HZ_KEY_S))
		{
			m_CameraPosition -= m_Camera.GetFront() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}

		if (Input::IsKeyPressed(HZ_KEY_LEFT) || Input::IsKeyPressed(HZ_KEY_A))
		{
			m_CameraPosition -= m_Camera.GetRight() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}
		else if (Input::IsKeyPressed(HZ_KEY_RIGHT) || Input::IsKeyPressed(HZ_KEY_D))
		{
			m_CameraPosition += m_Camera.GetRight() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}

		if (Input::IsKeyPressed(HZ_KEY_Q))
		{
			m_CameraPosition -= m_Camera.GetUp() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}
		else if (Input::IsKeyPressed(HZ_KEY_E))
		{
			m_CameraPosition += m_Camera.GetUp() * m_CameraTranslationSpeed * timestep.GetSeconds();
		}

		if (m_Rotation)
		{
			if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_RIGHT))
			{
				if (m_MouseFirstMoved)
				{
					m_LastMousePositionX = Input::GetMouseX();
					m_LastMousePositionY = Input::GetMouseY();
					m_MouseFirstMoved = false;
				}

				m_CursorOffsetX = Input::GetMouseX() - m_LastMousePositionX;
				m_CursorOffsetY = m_LastMousePositionY - Input::GetMouseY();

				m_LastMousePositionX = Input::GetMouseX();
				m_LastMousePositionY = Input::GetMouseY();

				// float cursorMaxOffset = 2;
				// m_CursorOffsetY = std::clamp(m_CursorOffsetY, -cursorMaxOffset, cursorMaxOffset);
				// m_CursorOffsetX = std::clamp(m_CursorOffsetX, -cursorMaxOffset, cursorMaxOffset);

				m_CameraRotation.x += m_CursorOffsetY * m_CameraRotationSpeed; // Pitch
				m_CameraRotation.y -= m_CursorOffsetX * m_CameraRotationSpeed; // Yaw



				// Pitch limit from -90 to 90 degrees
				if (m_CameraRotation.x > 89.0f)
					m_CameraRotation.x = 89.0f;
				if (m_CameraRotation.x < -89.0f)
					m_CameraRotation.x = -89.0f;
			}

			m_Camera.SetPosition(m_CameraPosition);
			m_Camera.SetRotation(m_CameraRotation);
		}

		if (Input::IsMouseButtonReleased(HZ_MOUSE_BUTTON_RIGHT))
		{
			m_MouseFirstMoved = true;
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		Hazel::EventDispatcher dispatcher(e);
		dispatcher.Dispatch<Hazel::MouseScrolledEvent>(HZ_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<Hazel::MouseMovedEvent>(HZ_BIND_EVENT_FN(CameraController::OnMouseMoved));
		dispatcher.Dispatch<Hazel::WindowResizeEvent>(HZ_BIND_EVENT_FN(CameraController::OnWindowResize));
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_CameraPosition.z -= e.GetYOffset() * m_CameraTranslationSpeed * 0.1f;
		m_Camera.SetPosition(m_CameraPosition);
		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (Input::IsMouseButtonPressed(HZ_MOUSE_BUTTON_1))
		{
			// HZ_INFO("Mouse position X = {0}, Y = {1}", e.GetX(), e.GetY());
		}

		return false;
	}

	bool CameraController::OnWindowResize(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

}
