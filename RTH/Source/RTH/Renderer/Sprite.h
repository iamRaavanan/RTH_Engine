#pragma once

#include "Texture.h"
#include "glm/glm.hpp"

namespace RTH
{
	class Sprite
	{
	public:
		Sprite(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
		static Ref<Sprite> CreateFromCoords(const Ref<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });

		const Ref<Texture2D> GetTexture() const { return mTexture; }
		const glm::vec2* GetTexCoords() const { return mTexCoords; }
	private:
		Ref<Texture2D> mTexture;
		glm::vec2 mTexCoords[4];
	};
}