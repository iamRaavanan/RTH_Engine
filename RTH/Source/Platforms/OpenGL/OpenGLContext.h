#pragma once
#include "RTH/Renderer/GraphicContext.h"

struct GLFWwindow;

namespace RTH
{
	class OpenGLContext : public GraphicContex
	{
	public:
		OpenGLContext(GLFWwindow* window);
		void virtual Init() override;
		void virtual SwapBuffers() override;

	private:
		GLFWwindow* mWindow;
	};
}