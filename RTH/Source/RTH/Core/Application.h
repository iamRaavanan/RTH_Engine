#pragma once
#include "Core.h"
#include "RTH/Event/Event.h"
#include "RTH/Event/ApplicationEvent.h"
#include "RTH/Core/LayerStack.h"
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
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> mWindow;
		ImGuiLayer* mImGuiLayer;
		bool mRunning;
		bool mIsMinimized = false;
		float mLastFrameTime;
		LayerStack mLayerStack;
	};
	Application* CreateApplication();
}