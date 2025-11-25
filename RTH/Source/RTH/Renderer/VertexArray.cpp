#include "Rthpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace RTH
{
	Ref<VertexArray> VertexArray::Create()
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
				return std::make_shared<OpenGLVertexArray>();
			}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}