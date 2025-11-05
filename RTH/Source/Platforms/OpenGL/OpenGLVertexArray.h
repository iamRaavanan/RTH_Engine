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

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return mVertexBuffer; }
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return mIndexBuffer; }
	private:
		std::vector<std::shared_ptr<VertexBuffer>> mVertexBuffer;
		std::shared_ptr<IndexBuffer> mIndexBuffer;
		uint32_t mRenderId;
	};
}