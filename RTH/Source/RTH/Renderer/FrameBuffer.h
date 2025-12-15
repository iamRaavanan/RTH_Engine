#pragma once
#include "RTH/Core/Core.h"

namespace RTH
{
	struct FrameBufferSpecification
	{
		uint32_t Width, Height;
		uint32_t Samples;
	};

	class FrameBuffer
	{
	public:

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;
		
		virtual uint32_t GetColorAttachmentRendererID() const = 0;
		virtual const FrameBufferSpecification& GetSpecification() const = 0;
		static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
	};
}