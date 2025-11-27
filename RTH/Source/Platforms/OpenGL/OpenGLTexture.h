#pragma once
#include "RTH/Renderer/Texture.h"
#include <glad/glad.h>

namespace RTH
{
	class OpenGLTexture2D : public Texture2D
	{
	public:
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(const std::string& path);
		~OpenGLTexture2D();
		virtual void SetData(void* data, uint32_t size) override;
		virtual uint32_t GetWidth() const override { return mWidth; };
		virtual uint32_t GetHeight() const override { return mHeight; };

		virtual void Bind(uint32_t slot = 0) const override;
	private:
		std::string mPath;
		uint32_t mWidth, mHeight;
		uint32_t mRendererID;
		GLenum mInternalFormat, mDataFormat;
	};
}
