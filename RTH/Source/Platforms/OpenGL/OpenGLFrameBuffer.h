#pragma once

#include "RTH/Renderer/FrameBuffer.h"

namespace RTH
{
	class OpenGLFrameBuffer : public FrameBuffer
	{
	public:
		OpenGLFrameBuffer(FrameBufferSpecification specification);
		virtual ~OpenGLFrameBuffer();

		void Invalidate();

		virtual void Bind() const override;
		virtual void UnBind() const override;
		virtual uint32_t GetColorAttachmentRendererID() const override { return mColorAttachment; }

		virtual const FrameBufferSpecification& GetSpecification() const override { return mSpecification; }
	private:
		uint32_t mRendererID;
		uint32_t mColorAttachment;
		uint32_t mDepthAttachment;
		FrameBufferSpecification mSpecification;
	};
}