#pragma once
#include "Core.h"
#include "Event/Event.h"
#include "Event/ApplicationEvent.h"
#include "RTH/LayerStack.h"
#include "Window.h"
#include "RTH/ImGui/ImGuiLayer.h"
#include "RTH/Renderer/Shader.h"
#include "RTH/Renderer/Buffer.h"

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
		std::unique_ptr<Shader> mShader;
		ImGuiLayer* mImGuiLayer;
		bool mRunning;

		LayerStack mLayerStack;

		unsigned int mVertexArray;
		std::unique_ptr<VertexBuffer> mVertexBuffer;
		std::unique_ptr<IndexBuffer> mIndexBuffer;
	};
	Application* CreateApplication();
}