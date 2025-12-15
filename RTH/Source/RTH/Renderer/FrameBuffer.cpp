#include "Rthpch.h"
#include "FrameBuffer.h"
#include "Renderer.h"
#include "Platforms/OpenGL/OpenGLFrameBuffer.h"

namespace RTH {
	Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec)
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
			return std::make_shared<OpenGLFrameBuffer>(spec);
		}
		}
		RTH_CORE_ASSERT(false, "Unknown RenderAPI!");
		return nullptr;
	}
}