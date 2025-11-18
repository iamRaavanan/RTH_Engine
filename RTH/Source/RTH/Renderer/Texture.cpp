#include "Rthpch.h"
#include "Texture.h"
#include "RTH/Renderer/Renderer.h"
#include "Platforms/OpenGL/OpenGLTexture.h"

namespace RTH
{
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
			return std::make_shared<OpenGLTexture2D>(path);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}