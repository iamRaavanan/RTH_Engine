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
		RTH_INFO("TestLayer::Update");
	}

	void OnEvent(RTH::Event& evnt) override
	{
		RTH_TRACE("{0}", evnt.ToString());
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