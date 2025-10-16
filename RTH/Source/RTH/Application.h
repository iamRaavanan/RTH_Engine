#pragma once
#include "Core.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "RTH/LayerStack.h"
#include "Window.h"

namespace RTH
{
	class RTH_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> mWindow;
		bool mRunning;

		LayerStack mLayerStack;
	};
	Application* CreateApplication();
}