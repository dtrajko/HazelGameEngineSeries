#pragma once

#include "SceneCamera.h"

#include <glm/glm.hpp>


namespace Hazel
{

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;

		TagComponent(const TagComponent&) = default;
		TagComponent(std::string tag)
			: Tag(tag) {};

		operator std::string& () { return Tag; };
		operator const std::string& () const { return Tag; };
	};

	struct TransformComponent
	{
		glm::mat4 Transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform)
			: Transform(transform) {};

		operator glm::mat4& () { return Transform; }; // implicit cast operator?
		operator const glm::mat4& () const { return Transform; }; // implicit cast operator?
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {};
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TODO: think about moving to Scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}
