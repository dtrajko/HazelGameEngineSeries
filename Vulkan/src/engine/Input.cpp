#include "Input.h"


Input::Input()
{
	currentPosition = glm::vec2();
	mouseX = 0.0;
	mouseY = 0.0;
}

Input* Input::get()
{
	static Input input;
	return &input;
}

void Input::update()
{
	for (int indexKey = 0; indexKey < GLFW_KEY_LAST; indexKey++)
	{
		m_Keys[indexKey] = isKeyPressed(indexKey);
	}
	for (int indexButton = 0; indexButton < GLFW_MOUSE_BUTTON_LAST; indexButton++)
	{
		m_MouseButtons[indexButton] = isMouseButtonPressed(indexButton);
	}
}

bool Input::isKeyPressed(unsigned int keyCode) const
{
	if (keyCode >= MAX_KEYS)
	{
		return false;
	}
	return m_Keys[keyCode];
}

bool Input::isMouseButtonPressed(unsigned int buttonCode) const
{
	if (buttonCode >= MAX_BUTTONS)
	{
		return false;
	}
	return m_MouseButtons[buttonCode];
}

void Input::setMousePosition(float xpos, float ypos)
{
	mouseX = xpos;
	mouseY = ypos;
	currentPosition.x = xpos;
	currentPosition.y = ypos;
}

Input::~Input()
{

}
