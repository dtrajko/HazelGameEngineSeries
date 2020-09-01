#pragma once

#include "Hazel/Scene/Scene.h"

namespace Hazel {

	class SceneRenderer
	{
	public:
		static void BeginScene(Scene* scene);
		static void SubmitEntity(Entity* entity);
		static void EndScene();
		static void CreateEnvironmentMap(const std::string& filepath);

	};

}
