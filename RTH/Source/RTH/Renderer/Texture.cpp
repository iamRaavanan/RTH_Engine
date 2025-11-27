#include "Rthpch.h"
#include "Texture.h"
#include "RTH/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace RTH
{
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
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
			return CreateRef<OpenGLTexture2D>(width, height);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::Create(const std::string& path)
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
			return CreateRef<OpenGLTexture2D>(path);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}