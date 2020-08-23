#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "EditorLayer.h"


namespace Hazel {

	class HazelnutApp : public Application
	{
	public:
		HazelnutApp()
			: Application("Hazelnut")
		{
			PushLayer(new EditorLayer());
		}

		~HazelnutApp()
		{
		}
	};

	Application* CreateApplication()
	{
		RendererAPI::SetAPI(RendererAPI::API::OpenGL);

		return new HazelnutApp();
	}

}
