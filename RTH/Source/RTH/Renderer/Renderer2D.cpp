#include "Rthpch.h"
#include "Renderer2D.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include<glm/gtc/matrix_transform.hpp>

namespace RTH
{
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
	};

	struct Renderer2DInfo
	{
		const uint32_t MaxQuads = 10000;
		const uint32_t MaxVertices = MaxQuads * 4;
		const uint32_t MaxIndices = MaxQuads * 6;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		uint32_t IndexCount;
	};

	static Renderer2DInfo sRendererInfo;

	void Renderer2D::Init()
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.vertexArray = VertexArray::Create();

		sRendererInfo.vertexBuffer = VertexBuffer::Create(sRendererInfo.MaxVertices * sizeof(QuadVertex));
		sRendererInfo.vertexBuffer->SetLayout({
				{ShaderDataType::Float3, "pos"},
				{ShaderDataType::Float4, "color"},
				{ShaderDataType::Float2, "texCoord"},
			});

		sRendererInfo.vertexArray->AddVertexBuffer(sRendererInfo.vertexBuffer);

		sRendererInfo.QuadVertexBufferBase = new QuadVertex[sRendererInfo.MaxVertices];
		uint32_t* indices = new uint32_t[sRendererInfo.MaxIndices];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < sRendererInfo.MaxIndices; i += 6)
		{
			indices[i + 0] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;
			offset += 4;
		}

		Ref<IndexBuffer> testSquareIB = IndexBuffer::Create(indices, sRendererInfo.MaxIndices);
		sRendererInfo.vertexArray->SetIndexBuffer(testSquareIB);
		delete[] indices;

		sRendererInfo.whiteTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		sRendererInfo.whiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		sRendererInfo.textureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sRendererInfo.textureShader->Bind();
		sRendererInfo.textureShader->SetInt("u_Tex", 0);
	}

	void Renderer2D::Shutdown()
	{
		RTH_PROFILE_FUNCTION();
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.textureShader->Bind();
		sRendererInfo.textureShader->SetMat4("u_ViewProj", camera.GetViewProjectionMat());
		sRendererInfo.IndexCount = 0;
		sRendererInfo.QuadVertexBufferPtr = sRendererInfo.QuadVertexBufferBase;
	}
	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint8_t*)sRendererInfo.QuadVertexBufferPtr - (uint8_t*)sRendererInfo.QuadVertexBufferBase;
		sRendererInfo.vertexBuffer->SetData(sRendererInfo.QuadVertexBufferBase, dataSize);

		RenderCommand::DrawIndexed(sRendererInfo.vertexArray, sRendererInfo.IndexCount);
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();

		sRendererInfo.QuadVertexBufferPtr->Position = position;
		sRendererInfo.QuadVertexBufferPtr->Color = color;
		sRendererInfo.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		sRendererInfo.QuadVertexBufferPtr++;

		sRendererInfo.QuadVertexBufferPtr->Position = { position.x + size.x, position.y, 0.0f};
		sRendererInfo.QuadVertexBufferPtr->Color = color;
		sRendererInfo.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		sRendererInfo.QuadVertexBufferPtr++;

		sRendererInfo.QuadVertexBufferPtr->Position = { position.x + size.x, position.y + size.y, 0.0f };
		sRendererInfo.QuadVertexBufferPtr->Color = color;
		sRendererInfo.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		sRendererInfo.QuadVertexBufferPtr++;

		sRendererInfo.QuadVertexBufferPtr->Position = { position.x, position.y + size.y, 0.0f };
		sRendererInfo.QuadVertexBufferPtr->Color = color;
		sRendererInfo.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		sRendererInfo.QuadVertexBufferPtr++;

		sRendererInfo.IndexCount += 6;
		/*sRendererInfo.textureShader->SetFloat4("u_Color", color);
		sRendererInfo.whiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererInfo.textureShader->SetMat4("u_Transform", transform);
		sRendererInfo.vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererInfo.vertexArray);*/
	}
	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, tilingMultiplier);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.textureShader->SetFloat4("u_Color", tintColor);
		sRendererInfo.textureShader->SetFloat("u_TilingMultiplier", tilingMultiplier);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererInfo.textureShader->SetMat4("u_Transform", transform);
		sRendererInfo.vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererInfo.vertexArray);
	}
	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.textureShader->SetFloat4("u_Color", color);
		sRendererInfo.textureShader->SetFloat("u_TilingMultiplier", 1.0f);
		sRendererInfo.whiteTexture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, {0.0f, 0.0f, 1.0f}) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererInfo.textureShader->SetMat4("u_Transform", transform);
		sRendererInfo.vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererInfo.vertexArray);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tintColor, tilingMultiplier);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.textureShader->SetFloat4("u_Color", tintColor);
		sRendererInfo.textureShader->SetFloat("u_TilingMultiplier", tilingMultiplier);
		texture->Bind();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0 });
		sRendererInfo.textureShader->SetMat4("u_Transform", transform);
		sRendererInfo.vertexArray->Bind();
		RenderCommand::DrawIndexed(sRendererInfo.vertexArray);
	}
}