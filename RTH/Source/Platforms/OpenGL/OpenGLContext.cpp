#include "Rthpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include "glad/glad.h"

namespace RTH
{
	OpenGLContext::OpenGLContext(GLFWwindow* window) : mWindow(window)
	{
		RTH_CORE_ASSERT(window, "Window is NULL!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(mWindow);
		int status = gladLoadGLLoader(GLADloadproc(glfwGetProcAddress));
		RTH_CORE_ASSERT(status, "Failed to initialize GLAD!");
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindow);
	}
}