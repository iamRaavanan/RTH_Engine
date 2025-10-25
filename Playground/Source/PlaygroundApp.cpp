#include "Rthpch.h"
#include <RTH.h>

class TestLayer : public RTH::Layer
{
public:
	TestLayer() : Layer("Test")
	{

	}

	void OnUpdate() override
	{
		if (RTH::Input::IsKeyPressed(RTH_KEY_TAB))
			RTH_TRACE("Tab key is pressed! via Poll");
	}

	void OnEvent(RTH::Event& evnt) override
	{
		//RTH_TRACE("{0}", evnt.ToString());
		if (evnt.GetEventType() == RTH::EventType::KeyPressed)
		{
			RTH::KeyPressedEvent& e = (RTH::KeyPressedEvent&)evnt;
			if (e.GetKeyCode() == RTH_KEY_TAB)
				RTH_TRACE("Tab key is pressed! via Event");
			RTH_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class PlaygroundApp : public RTH::Application
{
public:
	PlaygroundApp()
	{
		PushLayer(new TestLayer());
	}

	~PlaygroundApp()
	{

	}
};

RTH::Application* RTH::CreateApplication()
{
	return new PlaygroundApp();
}