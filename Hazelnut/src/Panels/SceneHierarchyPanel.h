#pragma once

#include "Hazel/Core/Base.h"
#include "Hazel/Core/Log.h"
#include "Hazel/Scene/Scene.h"
#include "Hazel/Scene/Entity.h"


namespace Hazel
{

	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& context);
		~SceneHierarchyPanel();

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		Ref<Scene> m_Context;

	};
}
