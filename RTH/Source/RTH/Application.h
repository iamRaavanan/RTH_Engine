#pragma once
#include "Core.h"
#include "Event/Event.h"
#include "Window.h"
namespace RTH
{
	class RTH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning;
	};
	Application* CreateApplication();
}