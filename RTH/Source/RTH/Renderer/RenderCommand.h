#pragma once
#include "RendererAPI.h"

namespace RTH
{
	class RenderCommand
	{
	public:
		inline static void SetClearColor(const glm::vec4& color)
		{
			sRendererAPI->SetClearColor(color);
		}

		inline static void Clear()
		{
			sRendererAPI->Clear();
		}

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			return sRendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* sRendererAPI;
	};
}