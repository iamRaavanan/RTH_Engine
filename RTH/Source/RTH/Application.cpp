#include "Application.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "Log.h"

namespace RTH
{
	Application::Application()
	{
	}
	Application::~Application()
	{
	}
	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		RTH_TRACE(e.ToString());
		while (true);
	}
}