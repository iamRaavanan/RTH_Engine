#include "Rthpch.h"
#include "OpenGLBuffer.h"
#include <glad/glad.h>

namespace RTH
{
	//================================ Vertex Buffer====================================
	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &mRenderId);
		Bind();
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &mRenderId);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, mRenderId);
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//================================ Index Buffer====================================

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		:mCount(count)
	{
		glCreateBuffers(1, &mRenderId);
		Bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &mRenderId);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mRenderId);
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}