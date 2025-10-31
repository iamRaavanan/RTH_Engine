#include "Rthpch.h"
#include "Buffer.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLBuffer.h"

namespace RTH
{
	VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size)
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
				return new OpenGLVertexBuffer(vertices, size);
			}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}

	IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetAPI())
		{
			case RenderAPI::None:
			{
				RTH_CORE_ASSERT(false, "RendererAPI::None is not supported!");
				return nullptr;
			}
			case RenderAPI::OpenGL:
			{
				return new OpenGLIndexBuffer(indices, size);
			}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}