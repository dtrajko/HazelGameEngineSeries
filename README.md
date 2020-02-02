# Hazel
Hazel Game Engine by TheCherno


Draft: Meshes, Materials, Material Instances
------------------------------------------------------------------------

Hazel::MaterialRef material = new Hazel::Material(m_FlatColorShader);
Hazel::MaterialInstanceRef mi = new Hazel::MaterialInstance(material);
mi->Set("u_Color", redColor);
material->Set("u_Texture", texture);
material->Set("u_AlbedoMap", albedoMap);
squareMesh->SetMaterial(mi);


Example 1: Responding to events
------------------------------------------------------------------------

void OnEvent(Hazel::Event& event) override
{
	if (event.GetEventType() == Hazel::EventType::KeyPressed)
	{
		Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
		if (e.GetKeyCode() == HZ_KEY_TAB)
			HZ_TRACE("Tab key is pressed (event)!");
		HZ_TRACE("{0}", (char)e.GetKeyCode());
	}
}


Example 2: Responding to keyboard events
------------------------------------------------------------------------

void OnEvent(Hazel::Event& event) override
{
	Hazel::EventDispatcher dispatcher(event);
	dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressedEvent));
}

// Responding to events - not in use, using input polling instead
bool OnKeyPressedEvent(Hazel::KeyPressedEvent& event)
{
	if (event.GetKeyCode() == HZ_KEY_LEFT || event.GetKeyCode() == HZ_KEY_A)
	{
		m_CameraPosition.x -= m_CameraMoveSpeed;
	}
	if (event.GetKeyCode() == HZ_KEY_RIGHT || event.GetKeyCode() == HZ_KEY_D)
	{
		m_CameraPosition.x += m_CameraMoveSpeed;
	}
	if (event.GetKeyCode() == HZ_KEY_UP || event.GetKeyCode() == HZ_KEY_W)
	{
		m_CameraPosition.y += m_CameraMoveSpeed;
	}
	if (event.GetKeyCode() == HZ_KEY_DOWN || event.GetKeyCode() == HZ_KEY_S)
	{
		m_CameraPosition.y -= m_CameraMoveSpeed;
	}
	return true;
}
