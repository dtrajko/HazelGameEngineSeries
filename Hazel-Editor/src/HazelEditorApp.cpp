#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Hazel {

	class HazelEditor : public Hazel::Application
	{
	public:

		HazelEditor()
		{
			PushLayer(new EditorLayer());
		}

		~HazelEditor()
		{
		}
	};

	Application* Hazel::CreateApplication()
	{
		RendererAPI::SetAPI(RendererAPI::API::OpenGL);

		return new HazelEditor();
	}

}
