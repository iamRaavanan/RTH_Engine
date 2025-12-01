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
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};
	static Renderer2DStorage* sRendererData;

	void Renderer2D::Init()
	{
		RTH_PROFILE_FUNCTION();
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

		sRendererData->whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		sRendererData->whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

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
		RTH_PROFILE_FUNCTION();
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
		RTH_PROFILE_FUNCTION();
		sRendererData->textureShader->SetFloat4("u_Color", color);
		sRendererData->whiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->textureShader->SetMat4("u_Transform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, tilingMultiplier);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		sRendererData->textureShader->SetFloat4("u_Color", tintColor);
		sRendererData->textureShader->SetFloat("u_TilingMultiplier", tilingMultiplier);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->textureShader->SetMat4("u_Transform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();
		sRendererData->textureShader->SetFloat4("u_Color", color);
		sRendererData->textureShader->SetFloat("u_TilingMultiplier", 1.0f);
		sRendererData->whiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->textureShader->SetMat4("u_Transform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tintColor, tilingMultiplier);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		sRendererData->textureShader->SetFloat4("u_Color", tintColor);
		sRendererData->textureShader->SetFloat("u_TilingMultiplier", tilingMultiplier);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererData->textureShader->SetMat4("u_Transform", transform);
		sRendererData->vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererData->vertexArray);
	}
}