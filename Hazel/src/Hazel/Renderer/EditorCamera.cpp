#include "hzpch.h"

#include "EditorCamera.h"


namespace Hazel {


	void EditorCamera::OnEvent(Event& e)
	{
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::vec3();
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::vec3();
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::vec3();
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat();
	}

	void EditorCamera::UpdateProjection()
	{
	}

	void EditorCamera::UpdateView()
	{
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
	}

	void EditorCamera::MouseZoom(float delta)
	{
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return glm::vec3();
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		return std::pair<float, float>();
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.0f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		return 0.0f;
	}

}
