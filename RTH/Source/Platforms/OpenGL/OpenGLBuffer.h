#pragma once
#include "RTH/Renderer/Buffer.h"

namespace RTH
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void UnBind() const override;

		virtual const BufferLayout& GetLayout() const override { return mBufferLayout; }
		virtual void SetLayout(const BufferLayout& layout) override { mBufferLayout = layout; }
	private:
		uint32_t mRenderId;
		BufferLayout mBufferLayout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void Bind() const;
		virtual void UnBind() const;

		virtual uint32_t GetCount() const { return mCount; }

	private:
		uint32_t mRenderId;
		uint32_t mCount;
	};
}