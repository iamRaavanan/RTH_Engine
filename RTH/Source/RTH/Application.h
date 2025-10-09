#pragma once
#include "Core.h"
namespace RTH
{
	class RTH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};
	Application* CreateApplication();
}