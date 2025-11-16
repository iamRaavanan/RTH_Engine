#pragma once
#include "RTH/Renderer/RendererAPI.h"

namespace RTH
{
	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		virtual void SetClearColor(const glm::vec4& color) override;
		virtual void Clear() override;
		virtual void DrawIndexed(const RTH::Ref<VertexArray>& vertexArray) override;
	};
}