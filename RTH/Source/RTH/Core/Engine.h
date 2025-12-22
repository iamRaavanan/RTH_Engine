#pragma once
#include "Core.h"
#include "RTH/Event/Event.h"
#include "RTH/Event/ApplicationEvent.h"
#include "RTH/Core/ModuleStack.h"
#include "Window.h"
#include "RTH/ImGui/ImGuiModule.h"

namespace RTH
{
	class RTH_API Engine
	{
	public:
		Engine(const std::string& name = "");
		virtual ~Engine();

		void Run();
		void OnEvent(Event& e);

		void PushModule(Module* Module);
		void PushOverlay(Module* Module);

		void Close() { mRunning = false; }
		
		ImGuiModule* GetImGuiModule() { return mImGuiModule; }

		inline static Engine& GetApplication() { return *sInstance; }
		inline Window& GetWindow() { return *mWindow; }
	private:
		static Engine* sInstance;
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		std::unique_ptr<Window> mWindow;
		ImGuiModule* mImGuiModule;
		bool mRunning;
		bool mIsMinimized = false;
		float mLastFrameTime;
		ModuleStack mModuleStack;
	};
	Engine* CreateApplication();
}