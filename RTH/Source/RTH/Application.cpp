#include "Rthpch.h"
#include "Application.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace RTH
{
	Application::Application()
	{
		mWindow = std::unique_ptr<Window>(Window::Create());
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		while (mRunning)
		{
			mWindow->OnUpdate();
		}
	}
}