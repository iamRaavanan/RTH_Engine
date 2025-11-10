#pragma once
#include "Core.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "RTH/LayerStack.h"
#include "Window.h"
#include "RTH/ImGui/ImGuiLayer.h"

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

		inline static Application& GetApplication() { return *sInstance; }
		inline Window& GetWindow() { return *mWindow; }
	private:
		static Application* sInstance;
		bool OnWindowClose(WindowCloseEvent& e);
		std::unique_ptr<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning;

		float mLastFrameTime;
		LayerStack mLayerStack;
	};
	Application* CreateApplication();
}