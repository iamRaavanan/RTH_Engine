#include "Rthpch.h"
#include "OpenGLVertexArray.h"
#include <glad/glad.h>

namespace RTH
{

	static GLenum ShaderDataTypesToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:	return GL_FLOAT;
		case ShaderDataType::Float3:	return GL_FLOAT;
		case ShaderDataType::Float4:	return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:		return GL_INT;
		case ShaderDataType::Int2:		return GL_INT;
		case ShaderDataType::Int3:		return GL_INT;
		case ShaderDataType::Int4:		return GL_INT;
		case ShaderDataType::Bool:		return GL_BOOL;
		}
		RTH_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		RTH_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &mRenderId);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		RTH_PROFILE_FUNCTION();
		glDeleteVertexArrays(1, &mRenderId);
	}

	void OpenGLVertexArray::Bind() const
	{
		RTH_PROFILE_FUNCTION();
		glBindVertexArray(mRenderId);
	}

	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const RTH::Ref<VertexBuffer>& vertexBuffer)
	{
		RTH_PROFILE_FUNCTION();
		RTH_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

		glBindVertexArray(mRenderId);
		vertexBuffer->Bind();
		
		uint32_t index = 0;
		const auto& layout = vertexBuffer->GetLayout();
		for (auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypesToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
		mVertexBuffer.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const RTH::Ref<IndexBuffer>& indexBuffer)
	{
		RTH_PROFILE_FUNCTION();
		glBindVertexArray(mRenderId);
		indexBuffer->Bind();
		mIndexBuffer = indexBuffer;
	}
}