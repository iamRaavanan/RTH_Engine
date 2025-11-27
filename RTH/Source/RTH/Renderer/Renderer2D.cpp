#include "Rthpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include<glm/gtc/matrix_transform.hpp>

namespace RTH
{
	struct Renderer2DStorage
	{
		Ref<VertexArray> vertexArray;
		Ref<Shader> flatshader;
		Ref<Shader> textureShader;
	};
	static Renderer2DStorage* sRendererData;

	void Renderer2D::Init()
	{
		sRendererData = new Renderer2DStorage();
		sRendererData->vertexArray = VertexArray::Create();

		float sqVertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> testSquareVB;
		testSquareVB = VertexBuffer::Create(sqVertices, sizeof(sqVertices));
		testSquareVB->SetLayout({
				{ShaderDataType::Float3, "pos"},
				{ShaderDataType::Float2, "texCoord"},
			});

		sRendererData->vertexArray->AddVertexBuffer(testSquareVB);

		uint32_t sqindices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> testSquareIB;
		testSquareIB.reset(IndexBuffer::Create(sqindices, sizeof(sqindices) / sizeof(uint32_t)));
		sRendererData->vertexArray->SetIndexBuffer(testSquareIB);

		sRendererData->flatshader = Shader::Create("Assets/Shaders/Flatcolor.glsl");
		sRendererData->textureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sRendererData->textureShader->Bind();
		sRendererData->textureShader->SetInt("u_Tex", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete sRendererData;
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		sRendererData->flatshader->Bind();
		sRendererData->flatshader->SetMat4("u_ViewProj", camera.GetViewProjectionMat());

		sRendererData->textureShader->Bind();
		sRendererData->textureShader->SetMat4("u_ViewProj", camera.GetViewProjectionMat());
	}
	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		sRendererData->flatshader->Bind();
		sRendererData->flatshader->SetFloat4("u_Color", color);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->flatshader->SetMat4("u_Transform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture)
	{
		sRendererData->textureShader->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->textureShader->SetMat4("u_Transform", transform);
		texture->Bind();
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
}