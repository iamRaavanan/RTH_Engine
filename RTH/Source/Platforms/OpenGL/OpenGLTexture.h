#pragma once
#include "RTH/Renderer/Texture.h"

namespace RTH
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();

		virtual uint32_t GetWidth() const override { return mWidth; };
		virtual uint32_t GetHeight() const override { return mHeight; };

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string mPath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
	};
}
