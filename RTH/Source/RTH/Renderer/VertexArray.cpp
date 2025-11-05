#include "Rthpch.h"
#include "VertexArray.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLVertexArray.h"

namespace RTH
{
	VertexArray* VertexArray::Create()
	{
		// Decide which Graphics API will be using
		switch (Renderer::GetAPI())
		{
			case RenderAPI::None:
			{
				RTH_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			}
			case RenderAPI::OpenGL:
			{
				return new OpenGLVertexArray();
			}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}