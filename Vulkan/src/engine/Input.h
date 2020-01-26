#pragma once

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>


#define MAX_KEYS  1024
#define MAX_BUTTONS 32


class Input
{
public:

	bool m_Keys[MAX_KEYS];
	bool m_MouseButtons[MAX_BUTTONS];
	float mouseX;
	float mouseY;
	float m_MouseWheelOffsetX;
	float m_MouseWheelOffsetY;
	glm::vec2 currentPosition;

public:

	Input();
	static Input* Get();
	void update();
	virtual ~Input();

	bool IsKeyPressed(unsigned int keyCode) const;
	bool IsMouseButtonPressed(unsigned int button) const;
	void SetMousePosition(float xpos, float ypos);

private:

	// bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
	// bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
	// bool OnMouseMovedEvent(MouseMovedEvent& e);
	// bool OnMouseScrolledEvent(MouseScrolledEvent& e);
	// bool OnKeyPressedEvent(KeyPressedEvent& e);
	// bool OnKeyReleasedEvent(KeyReleasedEvent& e);
	// bool OnKeyTypedEvent(KeyTypedEvent& e);
	// bool OnWindowResizeEvent(WindowResizeEvent& e);

};
