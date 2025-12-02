#include "Rthpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace RTH
{
	//================================ Vertex Buffer====================================
	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
	{
		RTH_PROFILE_FUNCTION();
		glCreateBuffers(1, &mRenderId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		RTH_PROFILE_FUNCTION();
		glCreateBuffers(1, &mRenderId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		RTH_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRenderId);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size)
	{
		RTH_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, mRenderId);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		RTH_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, mRenderId);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		RTH_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//================================ Index Buffer====================================

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:mCount(count)
	{
		RTH_PROFILE_FUNCTION();
		glCreateBuffers(1, &mRenderId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		RTH_PROFILE_FUNCTION();
		glDeleteBuffers(1, &mRenderId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		RTH_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderId);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		RTH_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}