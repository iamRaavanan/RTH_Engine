#include "Rthpch.h"
#include <RTH.h>

class PlaygroundApp : public RTH::Application
{
public:
	PlaygroundApp()
	{

	}

	~PlaygroundApp()
	{

	}
};

RTH::Application* RTH::CreateApplication()
{
	return new PlaygroundApp();
}