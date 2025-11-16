#pragma once
#include "RTH/Renderer/VertexArray.h"

namespace RTH
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		virtual ~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual void AddVertexBuffer(const RTH::Ref<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const RTH::Ref<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<RTH::Ref<VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffer; }
		virtual const RTH::Ref<IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; }
	private:
		std::vector<RTH::Ref<VertexBuffer>> mVertexBuffer;
		RTH::Ref<IndexBuffer> mIndexBuffer;
		uint32_t mRenderId;
	};
}