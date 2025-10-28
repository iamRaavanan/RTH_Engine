#pragma once
#include "RTH/Window.h"
#include "GLFW/glfw3.h"
#include "RTH/Renderer/GraphicContext.h"

namespace RTH {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return mData.Width; }
		inline unsigned int GetHeight() const override { return mData.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { mData.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const { return mWindow; }
	private:
		
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};

		GLFWwindow* mWindow;
		GraphicContex* mContext;
		WindowData mData;

		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	};
}