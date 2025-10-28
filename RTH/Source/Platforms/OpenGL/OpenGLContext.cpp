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

		RTH_CORE_TRACE("OpenGL Renderer : {0} - {1}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)), reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(mWindow);
	}
}