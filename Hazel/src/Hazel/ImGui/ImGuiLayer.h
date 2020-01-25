#pragma once

#include "Hazel/Layer.h"


namespace Hazel
{

	class ImGuiLayer : public Layer
	{
	public:

		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);


	private:



	};
}
