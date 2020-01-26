#include "Input.h"


Input::Input()
{
	currentPosition = glm::vec2();
	mouseX = 0.0;
	mouseY = 0.0;
}

Input* Input::Get()
{
	static Input input;
	return &input;
}

void Input::update()
{
	for (int indexKey = 0; indexKey < GLFW_KEY_LAST; indexKey++)
	{
		m_Keys[indexKey] = IsKeyPressed(indexKey);
	}
	for (int indexButton = 0; indexButton < GLFW_MOUSE_BUTTON_LAST; indexButton++)
	{
		m_MouseButtons[indexButton] = IsMouseButtonPressed(indexButton);
	}
}

bool Input::IsKeyPressed(unsigned int keyCode) const
{
	if (keyCode >= MAX_KEYS)
	{
		return false;
	}
	return m_Keys[keyCode];
}

bool Input::IsMouseButtonPressed(unsigned int buttonCode) const
{
	if (buttonCode >= MAX_BUTTONS)
	{
		return false;
	}
	return m_MouseButtons[buttonCode];
}

void Input::SetMousePosition(float xpos, float ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	currentPosition.x = xpos;
	currentPosition.y = ypos;
}

Input::~Input()
{

}
