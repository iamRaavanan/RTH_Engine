#include "Rthpch.h"
#include "SpriteRenderer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include<glm/gtc/matrix_transform.hpp>

namespace RTH
{
#define STATISTICS 1
	struct QuadVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;
		glm::vec2 TexCoord;
		float TexIndex;
		float TilingFactor;
	};

	struct SpriteRendererInfo
	{
		static const uint32_t MaxQuads = 10000;
		static const uint32_t MaxVertices = MaxQuads * 4;
		static const uint32_t MaxIndices = MaxQuads * 6;
		static const uint32_t MaxTextureSlots = 32;

		Ref<VertexArray> vertexArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;

		uint32_t IndexCount;
		QuadVertex* QuadVertexBufferBase = nullptr;
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
		uint32_t TextureSlotIndex = 1; // As, we usually start with 0. But on this case we set 0 for white texture
		glm::vec4 vertices[4];

		SpriteRenderer::Statistics stats;
	};

	static SpriteRendererInfo sRendererInfo;

	void SpriteRenderer::Init()
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.vertexArray = VertexArray::Create();

		sRendererInfo.vertexBuffer = VertexBuffer::Create(sRendererInfo.MaxVertices * sizeof(QuadVertex));
		sRendererInfo.vertexBuffer->SetLayout({
				{ShaderDataType::Float3, "pos"},
				{ShaderDataType::Float4, "color"},
				{ShaderDataType::Float2, "texCoord"},
				{ShaderDataType::Float, "texIndex"},
				{ShaderDataType::Float, "tileFactor"},
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

		int32_t samplers[sRendererInfo.MaxTextureSlots];
		for (uint32_t i = 0; i < sRendererInfo.MaxTextureSlots; i++)
			samplers[i] = i;

		sRendererInfo.textureShader = Shader::Create("Assets/Shaders/Texture.glsl");
		sRendererInfo.textureShader->Bind();
		sRendererInfo.textureShader->SetIntArray("u_Tex", samplers, sRendererInfo.MaxTextureSlots);

		sRendererInfo.TextureSlots[0] = sRendererInfo.whiteTexture;

		sRendererInfo.vertices[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		sRendererInfo.vertices[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
		sRendererInfo.vertices[2] = { 0.5f, 0.5f, 0.0f, 1.0f };
		sRendererInfo.vertices[3] = { -0.5f, 0.5f, 0.0f, 1.0f };
	}

	void SpriteRenderer::Shutdown()
	{
		RTH_PROFILE_FUNCTION();
	}

	void SpriteRenderer::BeginScene(const Camera2D& camera)
	{
		RTH_PROFILE_FUNCTION();
		sRendererInfo.textureShader->Bind();
		sRendererInfo.textureShader->SetMat4("u_ViewProj", camera.GetViewProjectionMat());
		sRendererInfo.IndexCount = 0;
		sRendererInfo.QuadVertexBufferPtr = sRendererInfo.QuadVertexBufferBase;
		sRendererInfo.TextureSlotIndex = 1;
	}
	void SpriteRenderer::EndScene()
	{
		RTH_PROFILE_FUNCTION();
		uint32_t dataSize = (uint8_t*)sRendererInfo.QuadVertexBufferPtr - (uint8_t*)sRendererInfo.QuadVertexBufferBase;
		sRendererInfo.vertexBuffer->SetData(sRendererInfo.QuadVertexBufferBase, dataSize);

		for (uint32_t i = 0; i < sRendererInfo.TextureSlotIndex; i++)
			sRendererInfo.TextureSlots[i]->Bind(i);
		RenderCommand::DrawIndexed(sRendererInfo.vertexArray, sRendererInfo.IndexCount);
#if STATISTICS
		sRendererInfo.stats.DrawCalls++;
#endif
	}

	void SpriteRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void SpriteRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		DrawQuad(transform, color);
	}

	void SpriteRenderer::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();

		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();

		const float textureIndex = 0.0f;

		const float tilingMultiplier = 1.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoord[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}

		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}
	void SpriteRenderer::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingMultiplier)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingMultiplier);
	}

	void SpriteRenderer::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture/*, const glm::vec4& tintColor*/, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		DrawQuad(transform, texture, tilingMultiplier);
	}

	void SpriteRenderer::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 texCoord[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < sRendererInfo.TextureSlotIndex; i++)
		{
			if (*sRendererInfo.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sRendererInfo.TextureSlotIndex;
			sRendererInfo.TextureSlots[sRendererInfo.TextureSlotIndex] = texture;
			sRendererInfo.TextureSlotIndex++;
		}

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}
		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}

	void SpriteRenderer::DrawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Sprite>& sprite, float tilingMultiplier)
	{
		DrawSprite({ position.x, position.y, 0.0f }, size, sprite/*, tintColor*/, tilingMultiplier);
	}

	void SpriteRenderer::DrawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Sprite>& sprite, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* texCoord = sprite->GetTexCoords();
		const Ref<Texture2D> texture = sprite->GetTexture();
		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < sRendererInfo.TextureSlotIndex; i++)
		{
			if (*sRendererInfo.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sRendererInfo.TextureSlotIndex;
			sRendererInfo.TextureSlots[sRendererInfo.TextureSlotIndex] = texture;
			sRendererInfo.TextureSlotIndex++;
		}
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * 
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}
		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}

	void SpriteRenderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, color);
	}

	void SpriteRenderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color)
	{
		RTH_PROFILE_FUNCTION();

		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();

		float textureIndex = 0.0f;
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec2 texCoord[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		float tilingMultiplier = 0.0f;
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });


		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}

		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}

	void SpriteRenderer::DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawRotatedQuad({ position.x, position.y, 0.0f }, size, rotation, texture, tintColor, tilingMultiplier);
	}

	void SpriteRenderer::DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		constexpr glm::vec2 texCoord[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < sRendererInfo.TextureSlotIndex; i++)
		{
			if (*sRendererInfo.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sRendererInfo.TextureSlotIndex;
			sRendererInfo.TextureSlots[sRendererInfo.TextureSlotIndex] = texture;
			sRendererInfo.TextureSlotIndex++;
		}
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}
		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}

	void SpriteRenderer::DrawRotatedSprite(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Sprite>& sprite, const glm::vec4& tintColor, float tilingMultiplier)
	{
		DrawRotatedSprite({ position.x, position.y, 0.0f }, size, rotation, sprite, tintColor, tilingMultiplier);
	}

	void SpriteRenderer::DrawRotatedSprite(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Sprite>& sprite, const glm::vec4& tintColor, float tilingMultiplier)
	{
		RTH_PROFILE_FUNCTION();
		if (sRendererInfo.IndexCount >= SpriteRendererInfo::MaxIndices)
			FlushAndReset();
		constexpr size_t quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		const glm::vec2* texCoord = sprite->GetTexCoords();
		const Ref<Texture2D> texture = sprite->GetTexture();

		float textureIndex = 0.0f;
		for (uint32_t i = 0; i < sRendererInfo.TextureSlotIndex; i++)
		{
			if (*sRendererInfo.TextureSlots[i].get() == *texture.get())
			{
				textureIndex = (float)i;
				break;
			}
		}
		if (textureIndex == 0.0f)
		{
			textureIndex = (float)sRendererInfo.TextureSlotIndex;
			sRendererInfo.TextureSlots[sRendererInfo.TextureSlotIndex] = texture;
			sRendererInfo.TextureSlotIndex++;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), rotation, { 0.0f, 0.0f, 1.0f }) *
			glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

		for (size_t i = 0; i < quadVertexCount; i++)
		{
			sRendererInfo.QuadVertexBufferPtr->Position = transform * sRendererInfo.vertices[i];
			sRendererInfo.QuadVertexBufferPtr->Color = color;
			sRendererInfo.QuadVertexBufferPtr->TexCoord = texCoord[i];
			sRendererInfo.QuadVertexBufferPtr->TexIndex = textureIndex;
			sRendererInfo.QuadVertexBufferPtr->TilingFactor = tilingMultiplier;
			sRendererInfo.QuadVertexBufferPtr++;
		}
		sRendererInfo.IndexCount += 6;

#if STATISTICS
		sRendererInfo.stats.QuadCount++;
#endif
	}
	void SpriteRenderer::ResetStats()
	{
		memset(&sRendererInfo.stats, 0, sizeof(Statistics));
	}

	SpriteRenderer::Statistics SpriteRenderer::GetStats()
	{
		return sRendererInfo.stats;
	}

	void SpriteRenderer::FlushAndReset()
	{
		EndScene();
		sRendererInfo.IndexCount = 0;
		sRendererInfo.QuadVertexBufferPtr = sRendererInfo.QuadVertexBufferBase;
		sRendererInfo.TextureSlotIndex = 1;
	}

}