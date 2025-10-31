#pragma once

namespace RTH
{
	enum class RenderAPI
	{
		None = 0, OpenGL = 1
	};

	class Renderer
	{
	public:
		inline static RenderAPI GetAPI() { return sRenderAPI; }
	private:
		static RenderAPI sRenderAPI;
	};
}