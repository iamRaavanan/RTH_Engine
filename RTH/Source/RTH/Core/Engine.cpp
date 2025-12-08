#include "Rthpch.h"
#include "Engine.h"
#include "RTH/Event/Event.h"
#include "Input.h"
#include "RTH/Renderer/Renderer.h"
#include "RTH/Core/Timestep.h"
#include <GLFW/glfw3.h>

namespace RTH
{
#define BIND_EVENT_FN(callback) std::bind(&Engine::callback, this, std::placeholders::_1)
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		RTH_PROFILE_FUNCTION();
		RTH_CORE_ASSERT(!sInstance, "Application already exist");
		sInstance = this;
		mRunning = true;
		mIsMinimized = false;
		mLastFrameTime = 0.0f;
		mWindow = std::unique_ptr<Window>(Window::Create());
		mWindow->SetEventCallback(BIND_EVENT_FN(OnEvent));
		Renderer::Init();
		mImGuiModule = new ImGuiModule();
		PushOverlay(mImGuiModule);
		
	}
	Engine::~Engine()
	{
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

		for (auto it = mModuleStack.end(); it != mModuleStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Engine::PushModule(Module* Module)
	{
		mModuleStack.PushModule(Module);
	}

	void Engine::PushOverlay(Module* Module)
	{
		mModuleStack.PushOverlay(Module);
		Module->OnAttach();
	}

	void Engine::Run()
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
					RTH_PROFILE_SCOPE("ModuleStack OnUpdate");
					for (Module* Module : mModuleStack)
						Module->OnUpdate(timestep);
				}
				

				{
					RTH_PROFILE_SCOPE("ModuleStack ImGUIRender");
					mImGuiModule->Begin();
					for (Module* Module : mModuleStack)
						Module->OnImGuiRender();
					mImGuiModule->End();
				}
			}
			mWindow->OnUpdate();
		}
	}

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}
	bool Engine::OnWindowResize(WindowResizeEvent& e)
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