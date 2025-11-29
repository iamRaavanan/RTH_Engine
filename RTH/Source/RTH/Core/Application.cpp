#include "Rthpch.h"
#include "Application.h"
#include "RTH/Event/Event.h"
#include "Input.h"
#include "RTH/Renderer/Renderer.h"
#include "RTH/Core/Timestep.h"
#include <GLFW/glfw3.h>

namespace RTH
{
#define BIND_EVENT_FN(callback) std::bind(&Application::callback, this, std::placeholders::_1)
	Application* Application::sInstance = nullptr;

	Application::Application()
	{
		RTH_PROFILE_FUNCTION();
		RTH_CORE_ASSERT(!sInstance, "Application already exist");
		sInstance = this;
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Renderer::Init();
		mImGuiLayer = new ImGuiLayer();
		PushOverlay(mImGuiLayer);
		
	}
	Application::~Application()
	{
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Run()
	{
		while (mRunning)
		{
			RTH_PROFILE_SCOPE("RunLoop");
			float time = glfwGetTime();
			Timestep timestep = time - mLastFrameTime;
			mLastFrameTime = time;

			if (!mIsMinimized)
			{
				{
					RTH_PROFILE_SCOPE("LayerStack OnUpdate");
					for (Layer* layer : mLayerStack)
						layer->OnUpdate(timestep);
				}
				

				{
					RTH_PROFILE_SCOPE("LayerStack ImGUIRender");
					mImGuiLayer->Begin();
					for (Layer* layer : mLayerStack)
						layer->OnImGuiRender();
					mImGuiLayer->End();
				}
			}
			mWindow->OnUpdate();
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			mIsMinimized = true;
			return false;
		}
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		mIsMinimized = false;
		return false;
	}
}