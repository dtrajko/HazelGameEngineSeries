# Hazel
Hazel Game Engine by TheCherno


Example: Responding to events

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
