#include "Rthpch.h"
#include "Shader.h"
#include "Renderer.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include "Platforms/OpenGL/OpenGLShader.h"

namespace RTH
{
	Shader* Shader::Create(const std::string& filePath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			RTH_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(filePath);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	Shader* Shader::Create(const std::string& vertexSrc, const std::string& fragSrc)
	{
		// Decide which Graphics API will be using
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			RTH_CORE_ASSERT(false, "RendererAPI::None is not supported!");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return new OpenGLShader(vertexSrc, fragSrc);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}